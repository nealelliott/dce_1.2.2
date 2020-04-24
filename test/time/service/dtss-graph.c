/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss-graph.c,v $
 * Revision 1.1.9.2  1996/02/17  23:32:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:50  marty]
 *
 * Revision 1.1.9.1  1995/12/11  23:06:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:37:19  root]
 * 
 * Revision 1.1.7.2  1994/08/24  14:38:42  cbrooks
 * 	CR11945 - undefined symbols
 * 	[1994/08/24  14:37:41  cbrooks]
 * 
 * Revision 1.1.7.1  1994/05/16  17:26:09  cbrooks
 * 	CR 10656
 * 	[1994/05/16  17:25:51  cbrooks]
 * 
 * 	CR 10656
 * 	[1994/05/16  16:13:03  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/19  00:47:33  jwade
 * 	Fixed the default path to the innacc.log file.
 * 	Fixed the statistics printout on the last page of the
 * 	graph so that it is readable.
 * 	OT CR 6892
 * 	[1993/01/19  00:46:51  jwade]
 * 
 * Revision 1.1.4.2  1992/12/31  20:36:01  htf
 * 	Embedded copyright notice
 * 	[1992/12/31  20:29:05  htf]
 * 
 * Revision 1.1.2.3  1992/02/10  16:42:38  eperkins
 * 	Made the edits to inline for real this time
 * 	[1992/02/10  16:42:15  eperkins]
 * 
 * Revision 1.1.2.2  1992/02/07  18:22:47  eperkins
 * 	Changed inline to  in_line because some compilers use this as a
 * 	token, and added casting * 	[1992/02/07  18:21:30  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:38:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module DTSS_GRAPH.C
 *
 * Copyright (c) 1989 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Wick Nichols
 *		Distributed Systems Services (DSS)
 *
et:* DATE:	September 1989
 *
 * -------------------------
 * Format of the input file:
 * -------------------------
 *      The first record has these fields:
 *        "Version %d Variant %s"
 *            where %d is the trace version number
 *                  %s is "Server" or "Clerk" (status of tracing node)
 *
 *      Each record beyond the first has these fields, separated by tabs:
 *      (all numbers are formatted in ASCII (printf (%d)))
 *        synchronization number
 *        one of the following pairs of fields:
 *         "0", server address, as a hex string; or name in text
 *         "1","Tsynch" for my record
 *         "2","Tcomp" for Tcomp record
 *         "3","separator" for separator between synchs
 *        Tcomp
 *        Tserv - Tcomp
 *        Iserv as estimated by clerk (incl round-trip and clerk drift)
 *        0 if fault not detected, 1 if fault detected in this synch
 *        # of records in this synchronization
 *        Iserv as reported by server
 *        1 if global set server, else 0
 * -------------------------
 */

#define CONFIG_THREADS	0      /* we don't init thread lib here, so must not */
			       /*  threads jacket macros for i/o calls */
#define CONFIG_RPC	1      /* Don't include RPC specific stuff in      */
			       /*  files                            */


typedef struct tm tm_t;		/* some call it tm, some call it tm_t */


#include <dce/dce.h>

#include <stdio.h>		/* C standard I/O library */

#include <dtssdef.h>		/* portable DTSS defintions */
#include <dtssmacros.h>		/* DTSS group-wide macros */

#include <math.h>
#include <string.h>
#include <utc.h>



/*
 * Data definitions
 */

#define GRAPHVERSION1 1
#define GRAPHVERSION2 2
#define GRAPHVERSION3 3
#define EXPECTEDGRAPHVERSION 4

#define MAXRECSPERPAGE 200
#define DEFRECSPERPAGE 100

#define MIN_VERT_SCALE (10 * 1e4)      /* 10 milliseconds */

#define DEFINPUT K_INACCFILE_NAME
#define DEFOUTPUT "dtss-graph.ps"


#define REMSRVCODE 0
#define LOCNODCODE 1
#define CALTIMCODE 2
#define SEPCODE 3

#define INCH 72
#define PAGEHEIGHT (8.5 * INCH)
#define YMARGIN (.5 * INCH)
#define YPIXELS (PAGEHEIGHT - (YMARGIN * 2) )
#define PAGEWIDTH (11 * INCH)
#define XMARGIN (.5 * INCH)
#define YAXISWIDTH (.5 * INCH)
#define XPIXELS (PAGEWIDTH - (XMARGIN * 2) - YAXISWIDTH)
#define TICKSIZE 4

#define SUMMARYFONTSIZE 12
#define SUMMARYSTATSIZE  9
#define SUMMARYTAB0 (XMARGIN        )
#define SUMMARYTAB1 (XMARGIN +  15.0)
#define SUMMARYTAB2 (XMARGIN +  30.0)
#define SUMMARYTAB3 (XMARGIN + 170.0)
#define STATTABSIZE 50

#define INFINACCPS "[3 1] 0 setdash"
#define FAULTPS "0.8 setgray"
#define IGNSTATGRAY 0.7

#define MAXNODES 26		/* one for each letter of alphabet */

#define MAXNAME 1024		/* Max length for a node name */
#define MAXLINE (MAXNAME+150)	/* Max length of trace record line */

/* 
 * Define constant designation difference in UTC and U*ix base times:
 *  DTSS UTC base time is October 15, 1582.
 *  UNIX base time is January 1, 1970.
 */
#define K_BASE_TIME_DIFF_LO   0x13814000	
#define K_BASE_TIME_DIFF_HI   0x01B21DD2
#define K_BASE_TIME_DIFF_F    ((K_BASE_TIME_DIFF_HI * 4294967296.0) + \
			       K_BASE_TIME_DIFF_LO)


typedef struct RecordElement
{
    double Tcomp;		/* computed time, as a tm struct */
    double Tdiff;		/* difference between Tserv and Tcomp */
    double Iservs;		/* server's inaccuracy as reported by server */
    double Iserv;		/* server's inaccuracy as est by clerk */
    long synchSerialNum;	/* latest synch this server used */
    int nodeId;			/* single-letter identifier for graph */
    unsigned long recordCount;	/* number of records in this synch */
    int code;			/* see REMSRVCODE, etc. */
    int fault;			/* 0=no fault, 1=fault in this synch */
    int includeInStats;		/* 0=no, 1=yes */
    int globalSet;		/* 0=no, 1=yes, server is in global set */
} RecordElement;

typedef struct NodeElement
{
    double sumSkews;		/* sum of skews */
    double sumSkews2;		/* sum of squares of skews */
    double maxSkews;		/* max of skews */
    double sumInaccs;		/* sum of inaccs */
    double sumInaccs2;		/* sum of squares of inaccs */
    double maxInaccs;		/* max of inaccs */
    double lastSkew;		/* last skew seen (for inflection calc) */
    unsigned long count;	/* count of records for this server  */
    char *nameP;		/* Pointer to node name */
    char useStats;		/* start using stats after skew inflection or -a switch */
    char globalSet;		/* 0=no, 1=yes, server is in global set */
    char seenFirst;		/* 1=we have seen first record for this node */
} NodeElement;

/*
 * Read server records into this array.
 * serverParray is a parallel array of pointers into serverArray for tag sort.
 */
static RecordElement serverArray[MAXRECSPERPAGE];
static RecordElement *(serverParray[MAXRECSPERPAGE]);

static int lastSeparator = 0;

#define MYNAMELEN ((size_t)MAXNAME)
static NodeElement myNodeElement;
static NodeElement nodeArray[MAXNODES];
static int numNodes;
static char myNodeName[MYNAMELEN];

/*
 * FILE pointers
 */
static FILE *infile;
static FILE *outfile;

/*
 * Other static global definitions
 */

static char autoCenter = 0;	/* calculate centering values from first page */
static char atOnly = 0;		/* local and calc times only */
static char statOnly = 0;	/* statistics only */
static char allStats = 0;	/* don't wait for inflection before */
				/* gathering stats */
static char usedInfInacc = 0;	/* put out infinacc summary */
static char usedFault = 0;	/* put out fault summary */

#define NO_TDF 187876
static long tdf = NO_TDF;	/* tdf in minutes */
static char tznam[80];		/* timezone name */
static int begPageNo = 0;	/* start printing at this page */
static int endPageNo = 0;	/* end printing at this page */

static int traceVersion;	/* version number in input trace file */

static double maxSkewPage;
static double maxInaccPage;

/*
 * Graph scaling values
 */
static double maxTime;		/* max (Tserv+Iserv) */
static double minTime;		/* min (Tserv-Iserv) */

static int numRecordsPerPage;
static double maxVerticalScale = 0;
static double centeringScale = 0;

static char *titleP;

static double firstTime = 0;
static double lastTime = 0;

static char variant[20];	/* "Server" or "Clerk" */

char inaccFilename[K_MAX_FILENAME+1];  /* for input filename */




/*
 * Macros
 */

/*
 * a local version of IsInfiniteInacc that works with floating reps of time
 */
#define IsInfInaccF(a) ((*(a) < 0) || (*(a) > 2.8147497e14))

#define SIGN(a) ((a) < 0 ? -1 : 1)


/*
 * Internal Prototypes
 */

static void UpdateMinMaxTime(RecordElement *) ;

static int ReadInputLine(RecordElement *) ;

static int LoadServerArray(void) ;

static void ScanServerArray(int ) ;

static int MakePostscriptGraph(void) ;

static int MakePostscriptSummary(void) ;

static int MakeSummaryPage(void) ;

static void PrintPage(int , int ) ;

static void MakePostsriptPage(int ,int) ;

static void LabelY(double *, double *) ;

int SortCompare(const void *, const void *) ;

static void WriteSummaryLine(int , NodeElement *, double ) ;

static void ReadTraceVersion(void) ;

static tm_t *ConvertFloatToTm(double );

void BuildLocalStats(RecordElement *) ;

void BuildNodeStats(RecordElement *, NodeElement *) ;

void FindNodeName(char *, int  *, int  *) ;

void WriteCenteredString(char *, double , double ) ;

void WriteRightedDouble(double , double , double ) ;

void ZeroStats(NodeElement *) ;

void SprintNodeName( char *, NodeAddr *, int, int) ;



static void PrintHelp (void)
{
    fprintf (stderr,
	     "usage:\tdtss-graph\n" );
    fprintf (stderr,
	     "\t\t[-f trace-file-name; default %s]\n",inaccFilename);
    fprintf (stderr,
	     "\t\t[-o output-ps-name; default %s]\n",DEFOUTPUT);
    fprintf (stderr,
	     "\t\t[-n number of records per page; ");
    fprintf (stderr,
	     "max %d, default %d]\n", MAXRECSPERPAGE, numRecordsPerPage);
    fprintf (stderr,
	     "\t\t[-t \"title in quotation marks\"; default input filename]\n");
    fprintf (stderr,
	     "\t\t[-z tdf; default is local timezone differential]\n");
    fprintf (stderr,
	     "\t\t[-b n;begin at page number n, default 1]\n");
    fprintf (stderr,
	     "\t\t[-e n;end at page number n, default 0 = no limit]\n");
    fprintf (stderr,
	     "\t\t[-@; only use local and calculated records]\n");
    fprintf (stderr,
	     "\t\t[-m n;max vertical scale in milliseconds, default 0 = no limit]\n");
    fprintf (stderr,
	     "\t\t[-c n; center zero line within n millisecond scale, 0 = calc from 1st page]\n");
    fprintf (stderr,
	     "\t\t[-s; only print summary statistics]\n");
    fprintf (stderr,
	     "\t\t[-a; gather all stats; default: wait for first inflection, ");
    fprintf (stderr,
	     "\t\t\t    to exclude initial stabilization period]\n");
    fprintf (stderr,
	     "\tOutput is a Postscript file\n");
    fprintf (stderr,
	     "\n\tTo trace synchronization activity:\n");
    fprintf (stderr,
	     "\t\trun the DTS control program and enter \"set synch trace true\".\n");
    fprintf (stderr,
	     "\t\tThis causes DTS to write a trace file for dtss-graph in\n");
    fprintf (stderr,
	     "\t\t%s.  DTS starts a new trace\n",DEFINPUT);
    fprintf (stderr,
	     "\t\tfile each time the service restarts.\n");
}



#define GETNEXTARG if (++i >= argc) { PrintHelp (); exit (1); }

int main (int   argc,char *argv[])
{
    char *innamP;
    char *outnamP;
    int i;
    unsigned int numPages;

    strcpy(inaccFilename, dcelocal_path);
    strcat(inaccFilename, K_INACCFILE_NAME);

    innamP = inaccFilename;
    outnamP = DEFOUTPUT;
    numRecordsPerPage = DEFRECSPERPAGE;
    titleP = 0;
    myNodeElement.nameP = myNodeName;

    if (argc < 2)
    {
	PrintHelp ();
	exit (1);
    }
    for (i=1 ; i<argc ; i++)
    {
	if ( argv[i][0] != '-')
	{
	    PrintHelp ();
	    exit (1);
	} else {
	    switch (argv[i][1])
	    {
	    /* 
	     * Accept upper case too, to make VMS happy
	     */
	    case 'F':
	    case 'f':
		GETNEXTARG;
		innamP = argv[i];
		break;

	    case 'O':
	    case 'o':
		GETNEXTARG;
		outnamP = argv[i];
		break;

	    case 'Z':
	    case 'z':
		GETNEXTARG;
		if (1 != sscanf (argv[i], "%d", &tdf))
		{
		    fprintf (stderr, "Bad tdf for -z\n");
		    PrintHelp ();
		    exit (1);
		}
		tznam[0] ='\0';
		break;

	    case 'B':
	    case 'b':
		GETNEXTARG;
		if (1 != sscanf (argv[i], "%d", &begPageNo))
		{
		    fprintf (stderr, "Bad beginning page number for -b\n");
		    PrintHelp ();
		    exit (1);
		}
		break;

	    case 'E':
	    case 'e':
		GETNEXTARG;
		if (1 != sscanf (argv[i], "%d", &endPageNo))
		{
		    fprintf (stderr, "Bad ending page number for -e\n");
		    PrintHelp ();
		    exit (1);
		}
		break;

	    case '@':
		atOnly = 1;
		break;

	    case 'S':
	    case 's':
		statOnly = 1;
		break;

	    case 'A':
	    case 'a':
		allStats = 1;
		break;

	    case 'T':
	    case 't':
		GETNEXTARG;
		titleP = argv[i];
		break;

	    case 'M':
	    case 'm':
		GETNEXTARG;
		sscanf (argv[i], "%lf", &maxVerticalScale);
		if (maxVerticalScale < 0)
		{
		    fprintf (stderr,
			     "Illegal max vertical scale: %f\n",
			     maxVerticalScale);
		    PrintHelp ();
		    exit (1);
		}
		maxVerticalScale *= 1e4; /* convert milliseconds into 100ns */
		break;

	    case 'C':
	    case 'c':
		GETNEXTARG;
		sscanf (argv[i], "%lf", &centeringScale);
		if (centeringScale == 0)
		{
		    autoCenter = 1;
		}
		else
		{
		    if (centeringScale < 0)
		    {
			fprintf (stderr,
				 "Illegal centering scale: %f\n",
				 centeringScale);
			PrintHelp ();
			exit (1);
		    }
		    centeringScale *= 1e4; /* convert milliseconds into 100ns */
		}
		break;

	    case 'N':
	    case 'n':
		GETNEXTARG;
		sscanf (argv[i], "%d", &numRecordsPerPage);
		if (numRecordsPerPage <= 0)
		{
		    fprintf (stderr,
			     "Illegal number of records per page: %d\n",
			     numRecordsPerPage);
		    PrintHelp ();
		    exit (1);
		}
		if (numRecordsPerPage > MAXRECSPERPAGE)
		{
		    fprintf (stderr,
			     "Too many records per page (-n %d), using %d\n",
			     numRecordsPerPage, MAXRECSPERPAGE);
		    numRecordsPerPage = MAXRECSPERPAGE;
		}
		break;

	    default:
		PrintHelp ();
		exit(1);
	    }
	}
    }

    /*
     * Open the input and output files.
     */
#ifdef whyisthat
    /*
     * Open input file "rw" to allow the service to keep writing to it.
     * RAH = allow read ahead
     */
    if ((infile = fopen (innamP, "rw, rop=RAH")) == (FILE *) 0)
#else
    if ((infile = fopen (innamP, "r")) == (FILE *) 0)
#endif
    {
	fprintf (stderr, "error opening %s for input\n", innamP);
	perror ("  error");
	exit(1);
    }
#ifdef whyisthat
    /*
     * WBH = write behind
     */
    if ((outfile = fopen (outnamP, "w, rop=WBH")) == (FILE *) 0)
#else
    if ((outfile = fopen (outnamP, "w")) == (FILE *) 0)
#endif
    {
	fprintf (stderr, "error opening %s for output\n", outnamP);
	perror ("  error");
	exit(1);
    }
    
    /*
     * Default the title to the input filename
     */
    if (titleP == 0)
	titleP = innamP;

    /*
     * ReadTraceVersion fills the global traceVersion with input
     * file's version number and the global string Variant with
     * "Server" or "Clerk" or null, from the first line of the trace.
     */
    ReadTraceVersion ();
    if (traceVersion > EXPECTEDGRAPHVERSION)
    {
	fprintf (stderr,
		 "Incompatible trace file version number (%d), ",
		 traceVersion);
	fprintf (stderr,
		 "expecting version %d\n", EXPECTEDGRAPHVERSION);
	fprintf (stderr,
		 "Continuing, but output may not be meaningful\n");
    }

    numPages =  MakePostscriptGraph ();

    numPages += MakePostscriptSummary ();

#if 0
    /* Someday we may get around to putting out the cannonical comments
     * that Adobe expects at the beginning and end of Postscript files.
     */
    PrintEndComments (numPages);
#endif

    fclose (infile);
    fclose (outfile);
    return (0);
}



/*
 * Returns number of pages printed
 */

static int MakePostscriptGraph (void)
{
    int pageNo, numPagesPrinted, numRecords;

    /*
     * Emit postscript header
     */
    fprintf (outfile, "%%!\n");
    fprintf (outfile, "%%%%Title: %s\n", titleP);
    fprintf (outfile, "%%%%EndComments\n");
    fprintf (outfile, "\n");
    /*
     * Loop over output pages
     */
    if (endPageNo == 0)
	endPageNo = 65000;
    numPagesPrinted = 0;
    pageNo = 1;
    while (pageNo <= endPageNo)
    {
	/*
	 * Load trace file into serverArray.
	 * Side effects: update maxTime and minTime global variables
	 */
	if (! (numRecords = LoadServerArray ()) )
	    break;

	if (pageNo >= begPageNo)
	{
	    ScanServerArray (numRecords);
	    if (! statOnly)
	    {
		PrintPage (numRecords, pageNo);
		numPagesPrinted++;
	    }
	}
	pageNo++;
    }
    return (numPagesPrinted);
}



static void PrintPage (int numRecords, int pageNo)
{
    /*
     * Put out a page number comment
     */
    fprintf (outfile, "%%%% Page %d\n",pageNo);
    /*
     * Save state of Postscript VM, restore at end of page
     */
    fprintf (outfile, "\n%% Save state of VM, restore at end of page\n");
    fprintf (outfile, "/dtss-save save def\n");
    /*
     * Write out a page. Keep going until MakePostsriptPage reports
     * that it has put out the last page.
     */
    MakePostsriptPage (numRecords, pageNo);
    
    fprintf (outfile, "showpage\n");
    
    /*
     * Restore state of Postscript VM, start clean on next page
     */
    fprintf (outfile, "\n%% Restore state of VM\n");
    fprintf (outfile, "dtss-save restore\n");
}



/*
 * Returns number of pages printed
 */
static int MakePostscriptSummary (void)
{
    int numPages;

    /*
     * Put out a summary number comment
     */
    fprintf (outfile, "%%%% Summary Page\n");
    /*
     * Save state of Postscript VM, restore at end of page
     */
    fprintf (outfile, "\n%% Save state of VM, restore at end of page\n");
    fprintf (outfile, "/dtss-save save def\n");
    /*
     * Write out the summary page.
     */
    numPages = MakeSummaryPage ();
    
    fprintf (outfile, "showpage\n");
    
    /*
     * Restore state of Postscript VM, start clean on next page
     */
    fprintf (outfile, "\n%% Restore state of VM\n");
    fprintf (outfile, "dtss-save restore\n");

    return (numPages);
}


/*
 * no return, int is there for easy porting
 */

static void MakePostsriptPage (int numRecords, int pageNo)
{
    double xPos, yPos;
    double xAxisPos, startXpos;
    int i, intemp, timeFontSize;
    double scaleFactor;		/* derive pixel lengths of graph lines */
    double spacing;		/* spacing between graph lines */
    RecordElement *reP;
    tm_t tm_time;

    /*
     * Set font to 12 point
     *
     * Print the y-axis title
     */
    fprintf (outfile, "%% Print y-axis title, centered\n");
    fprintf (outfile, "/Times-Roman findfont 12 scalefont setfont\n");
    intemp = PAGEHEIGHT;
    fprintf (outfile, "180 rotate -%d 0 translate\n",intemp);
    fprintf (outfile, "(M i l l i s e c o n d s) dup stringwidth pop\n");
    intemp = PAGEHEIGHT / 2;
    xPos = XMARGIN - 2;
    fprintf (outfile, "2 div neg %d add -%1.2f moveto show\n", intemp, xPos);
    fprintf (outfile, "\n");
    /*
     * Rotate to landscape format
     */
    fprintf (outfile, "%% Rotate to landscape format\n");
    fprintf (outfile, "-90 rotate\n");
    fprintf (outfile, "\n");
    /*
     * Put out the title, if user gave us one
     */
    yPos = PAGEHEIGHT - (YMARGIN/2) - 3;
    /*
     * The variant
     */
    if (strlen (variant) > 0)
    {
	xPos = XMARGIN + YAXISWIDTH;
	fprintf (outfile, "%1.2f %1.2f moveto (%s)show\n",
		 xPos, yPos, variant);
    }
    /*
     * The title (if there is one)
     */
    if (titleP)
    {
	xPos = XMARGIN + YAXISWIDTH + (XPIXELS/2);
	fprintf (outfile, "(%s) dup stringwidth pop ", titleP);
	fprintf (outfile, "2 div neg %1.2f add %1.2f moveto show\n",
		 xPos, yPos);
    }
    /*
     * The date
     */
    tm_time = *ConvertFloatToTm (lastTime);
    xPos = PAGEWIDTH - XMARGIN;
    fprintf (outfile, "(%d-%d-%d) dup stringwidth pop ",
	     tm_time.tm_year + 1900,
	     tm_time.tm_mon + 1,
	     tm_time.tm_mday);
    fprintf (outfile, "neg %1.2f add %1.2f moveto show\n",
	     xPos, yPos);
    /*
     * The page number
     */
    xPos = XMARGIN + YAXISWIDTH + (XPIXELS/2);
    yPos = YMARGIN / 4;
    fprintf (outfile, "(Page %d) dup stringwidth pop ", pageNo);
    fprintf (outfile, "2 div neg %1.2f add %1.2f moveto show\n",
	     xPos, yPos);
    /*
     * Max skew and inaccuracy
     */
    fprintf (outfile, "/Times-Roman findfont 9 scalefont setfont\n");
    xPos = XMARGIN + YAXISWIDTH;
    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
    fprintf (outfile,
	     "(Max local skew from calculated time is %1.2f ms)show\n",
	     maxSkewPage / 1e4);
    xPos = PAGEWIDTH - XMARGIN;
    if (IsInfInaccF (&maxInaccPage))
	fprintf (outfile, "(Max local inaccuracy is infinite)\n");
    else
	fprintf (outfile, "(Max local inaccuracy is %1.2f ms)\n",
		 maxInaccPage / 1e4);
    fprintf (outfile, "dup stringwidth pop neg %1.2f add %1.2f moveto show\n",
	     xPos, yPos);
    /*
     * Calculate the X-axis scaling.
     * +1 leaves room on right for right-hand Y-axis.
     */
    spacing = XPIXELS / (numRecords + 1);
    /*
     * Calculate the Y-axis scaling and draw the Y-axis
     */
    LabelY (&scaleFactor, &xAxisPos);
    /*
     * Draw the graph
     */
    timeFontSize = (numRecords > DEFRECSPERPAGE) ? 6 : 8;
    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     timeFontSize);
    xPos = XMARGIN + YAXISWIDTH + spacing;
    startXpos = xPos;
    fprintf (outfile, "%% Draw the graph\n");
    for (i=0 ; i < numRecords ; i++ )
    {
	reP = serverParray[i];	       /* pointer into server array */
	/*
	 * If this is not a spacer record, put out a line
	 */
	if (reP->code == SEPCODE)
	{
	    startXpos = xPos + spacing;	/* next is start of new synch */
	} else {
	    if (IsInfInaccF (&reP->Iserv))
		yPos = YMARGIN + YPIXELS;
	    else
		yPos = xAxisPos + scaleFactor * (reP->Tdiff + reP->Iserv);
	    if ((reP->code == REMSRVCODE) ||
		(reP->code == LOCNODCODE))
	    {
		char c;
		/*
		 * Print the node's id letter
		 */
		if (reP->code == LOCNODCODE)
		    c = '@';
		else
		{
		    if (reP->nodeId == -1)
			c = '?';
		    else
			c = reP->nodeId + (reP->globalSet ? 'A' : 'a');
		}
		/*
		 * Make a little white box to fit the letter in, so
		 * the letter won't be obscured by the grid lines.
		 */
		fprintf (outfile, "newpath (%c) stringwidth pop ", c);
		fprintf (outfile, "dup /dtscwid exch def 2 div neg ");
		fprintf (outfile, "%1.2f add %1.2f moveto\n",
			 xPos, yPos);
		fprintf (outfile, "0  %d rlineto dtscwid 0 rlineto ",
			 timeFontSize);
		fprintf (outfile, "0 -%d rlineto closepath ",
			 timeFontSize);
		fprintf (outfile, "1 setgray fill %1.1f setgray\n",
			 (reP->includeInStats ? 0 : IGNSTATGRAY) );
		fprintf (outfile,
			 "dtscwid 2 div neg %1.2f add %1.2f moveto\n",
			 xPos, yPos + 2);
		fprintf (outfile, "(%c)show 0 setgray\n", c);
	    }
	    /*
	     * Draw the inaccuracy line
	     */
	    fprintf (outfile, "gsave\n"); /* , xPos, yPos); */

	    fprintf (outfile, "newpath %1.2f %1.2f moveto ", xPos, yPos);
	    if (IsInfInaccF (&reP->Iserv))
	    {
		yPos = YMARGIN;
		usedInfInacc = 1;
		/* make a dashed line */
		fprintf (outfile, "\n%s ", INFINACCPS);
	    } else
		yPos = xAxisPos + scaleFactor * (reP->Tdiff - reP->Iserv);
	    fprintf (outfile, "%1.2f %1.2f lineto ", xPos, yPos);
	    switch (reP->code)
	    {
            case CALTIMCODE:

                fprintf (outfile, "\n gsave ");
		/* 
		 * Old versions don't put out two inaccs for calc'd time
		 */
		if (traceVersion < 4)
		    fprintf (outfile, " 2.0 setlinewidth ");
                if (reP->fault)
                {
                    usedFault = 1;
                    fprintf (outfile,"%s ", FAULTPS);
                }
		/*
		 * Print the thin line
		 */
                fprintf (outfile, "stroke ");
		/* 
		 * If we need to, paint a thick line over it
		 */
		if ((reP->Iservs > 0)   &&
		    (traceVersion >= 4) &&
		    ( ! IsInfInaccF (&reP->Iserv) ))
		{
		    yPos = xAxisPos + scaleFactor * (reP->Tdiff + reP->Iservs);
		    fprintf (outfile,
			     "\nnewpath %1.2f %1.2f moveto ", xPos, yPos);
		    yPos = xAxisPos + scaleFactor * (reP->Tdiff - reP->Iservs);
		    fprintf (outfile, "%1.2f %1.2f lineto\n", xPos, yPos);
		    fprintf (outfile,
			     "2.0 setlinewidth stroke ");
		}
                fprintf (outfile, "grestore\n");
		break;

            case LOCNODCODE:
                fprintf (outfile,
			 "\ngsave 2.0 setlinewidth stroke grestore\n");
		break;

	    case REMSRVCODE:
                fprintf (outfile, "stroke\n");
		/*
		 * Stroke a double-width line over the server's contribution to
		 * the inaccuracy. The remaining single-width line is the extra
		 * inaccuracy due to round-trip time and drift in the clerk.
		 */
		if ((reP->Iservs > 0) &&
		    ( ! IsInfInaccF (&reP->Iserv) ))
		{
		    yPos = xAxisPos + scaleFactor * (reP->Tdiff + reP->Iservs);
		    fprintf (outfile,
			     "newpath %1.2f %1.2f moveto ", xPos, yPos);
		    yPos = xAxisPos + scaleFactor * (reP->Tdiff - reP->Iservs);
		    fprintf (outfile, "%1.2f %1.2f lineto\n", xPos, yPos);
		    fprintf (outfile,
			     "gsave 2.0 setlinewidth stroke grestore\n");
		}
		break;

            default:
                fprintf (outfile, "stroke\n");
		break;
            }

	    /*
	     * Make the midpoint tick, unless it's out of bounds
	     */
	    yPos = xAxisPos + (scaleFactor * reP->Tdiff);
	    fprintf (outfile, "newpath %1.2f %1.2f moveto ",xPos,yPos);
	    fprintf (outfile, "%1.2f %1.2f lineto stroke\n",
		     (xPos + (spacing/2)), yPos);

	    fprintf (outfile,"grestore\n");
	}
	if (reP->code == CALTIMCODE)
	{
	    double locInacc;

	    /* 
	     * Old traces only put out a single inacc for calc times, 
	     * new traces put out two.  For new traces, draw the 
	     * intersection lines at the ends of the narrow calc'd 
	     * time inacc, not the wider basetime inacc.
	     */
	    locInacc = (traceVersion < 4) ? reP->Iserv : reP->Iservs;

	    /*
	     * Draw the top intersection line
	     */
	    fprintf (outfile, "%% Draw the intersection lines\n");
	    yPos = xAxisPos + scaleFactor * (reP->Tdiff + locInacc);
	    fprintf (outfile, "newpath %1.2f %1.2f moveto ",xPos,yPos);
	    fprintf (outfile, "%1.2f %1.2f lineto\n", startXpos, yPos);
	    fprintf (outfile, "[1 1] 0 setdash .5 setlinewidth stroke\n");
	    /*
	     * now the bottom intersection line
	     */
	    yPos = xAxisPos + scaleFactor * (reP->Tdiff - locInacc);
	    fprintf (outfile, "newpath %1.2f %1.2f moveto ",xPos,yPos);
	    fprintf (outfile, "%1.2f %1.2f lineto stroke\n", startXpos, yPos);
	    fprintf (outfile, "[] 0 setdash\n");
	    /*
	     * Draw a tick, then write the time
	     * along the bottom of the page.
	     */
	    yPos = YMARGIN - TICKSIZE;
	    fprintf (outfile, "newpath %1.2f %1.2f moveto ", xPos, yPos);
	    intemp = TICKSIZE;
	    fprintf (outfile, "0 %d rlineto stroke\n", intemp);
	    tm_time = *ConvertFloatToTm (reP->Tcomp);
	    if (numRecords >= 150)
	    {
		fprintf (outfile, "(%02d) ",
			 tm_time.tm_min);
	    } else {
		fprintf (outfile, "(%d:%02d) ",
			 tm_time.tm_hour,
			 tm_time.tm_min);
	    }
	    fprintf (outfile, "dup stringwidth pop ");
	    fprintf (outfile, "2 div neg %1.2f add %1.2f moveto show\n",
		     xPos, yPos - timeFontSize);
	    fprintf (outfile, "1 setlinewidth\n");
	    fprintf (outfile, "\n");
	}
	xPos += spacing;
    }
}



/*
 * Load up the server array from the input file. Also build a parallel
 * array (serverParray) of pointers into the server array. Sort the
 * parallel array so that the remote servers in each synchronization
 * always show up in the same order.
 *
 * If the input file is longer than numRecordsPerPage elements, leave
 * the rest for the next page.  On the way, compute the max and min
 * excursions of the time intervals so we can scale the graph to fit.
 *
 * Global outputs:
 *    maxTime = max (Tdiff + Iserv) in this set of records
 *    minTime = min (Tdiff - Iserv) in this set of records
 *
 * Returns:
 *    int numRecords = number of records to print on this page.
 *                     Zero means end of input file.
 */

static int LoadServerArray (void)
{
    int numRecords, i;
    RecordElement *reP;
    static int readResult;

    /*
     * If we ended last time by reading EOF, return zero records
     * this time.
     */
    if (readResult == EOF) return (0);

    /*
     * Copy the unused records from last page to the beginning of the
     * server array. Set lastSeparator and numRecords to the end of
     * the copied range.
     */
    if (lastSeparator == 0)
    {
	numRecords = 0;
    } else {
	numRecords = numRecordsPerPage - lastSeparator;
	for (i=numRecords-1 ; i >= 0 ; i--)
	{
	    serverArray[i] = serverArray[lastSeparator + i];
	}
    }
    /*
     * Load in more servers from the input file, until EOF or until the
     * array is full.
     */
    reP = &serverArray[numRecords];
    /*
     * if first record is a separator, ignore it
     */
    readResult = EOF + 1;	/* something other than EOF */
    if (numRecords == 0)
    {
	if (EOF != (readResult = ReadInputLine (reP)))
	{
	    if (reP->code != SEPCODE)
	    {
		if ( (! atOnly) || (reP->code != REMSRVCODE))
		{
		    numRecords++;
		    reP++;
		}
	    }
	}
    }
    if (readResult != EOF)
    {
	while (numRecords < numRecordsPerPage)
	{
	    if (EOF == (readResult = ReadInputLine (reP)))
		break;
	    if ( (! atOnly) || (reP->code != REMSRVCODE))
	    {
		numRecords++;
		reP++;
	    }
	}
    }
    /*
     * Find last separator
     */
    lastSeparator = -1;
    reP = serverArray;
    for (i=0 ; i < numRecords ; i++)
    {
	if (reP->code == SEPCODE)
	    lastSeparator = i;
	reP++;
    }
    /*
     * If we found a separator record, return a record count including
     * all records up to but not including the last separator.
     * numRecords is a 1-relative count,
     * lastSeparator is a 0-relative offset.
     */
    if (lastSeparator != -1)
	numRecords = lastSeparator;

    return (numRecords);		 /* return # of records found */
}



static void ScanServerArray(int numRecords)
{
    int numSynchs, i;
    RecordElement *reP;

    /*
     * Find max and min times, and find the last synch separator
     */
    maxTime = minTime = 0;	/* global variables */
    numSynchs = 0;
    maxSkewPage = maxInaccPage = 0;
    reP = serverArray;
    for ( i=0 ; i < numRecords ; i++,reP++ )
    {
	/* Build the parallel array of pointers for sorting
	 */
	serverParray[i] = reP;

	/*
	 * Construct new synchSerialNos, incrementing on every separator
	 * so that the double sets output during a checkint are separated
         * in the sort.
	 */
	reP->synchSerialNum = numSynchs;
	switch (reP->code)
	    {
	    case SEPCODE:
		numSynchs++;
		break;
	
	    case CALTIMCODE:
		UpdateMinMaxTime (reP);
		lastTime   = reP->Tcomp;
		if (firstTime == 0)
		    firstTime = reP->Tcomp;
		break;

	    case LOCNODCODE:
		UpdateMinMaxTime (reP);
		BuildLocalStats (reP);
		BuildNodeStats  (reP, &myNodeElement);
		break;

	    case REMSRVCODE:
		UpdateMinMaxTime (reP);
		BuildNodeStats (reP, &nodeArray [reP->nodeId]);
		break;
	    }
    }
    /*
     * Sort the array, so that the remote servers come out in the same
     * order in each synchronization. There is enough info in the records
     * (synchSerialNum, code and nodeId) to allow the sort to keep
     * the synchs in order and the remote servers before the local
     * server before the calculated time.
     *
     * We actually sort a parallel array of pointers into the main array,
     * to avoid a bug in qsort that ACCVIOs when the array element size
     * is > 40. Also, this tag sort allows qsort to swap 4-byte pointers
     * instead of 60-byte elements.
     */
    if (numRecords > 0)
    {
	qsort ((char *) serverParray,	 /* ptr to base of array */
	       numRecords,		 /* number of elements */
	       sizeof (RecordElement *), /* size of an element */
	       SortCompare);		 /* ptr to comparison routine */
    }
}

int SortCompare (const void *av, const void *bv)
{
    RecordElement *a, *b;

    a = *(RecordElement **) av;
    b = *(RecordElement **) bv;

    if (a->synchSerialNum > b->synchSerialNum) return ( 1);
    if (a->synchSerialNum < b->synchSerialNum) return (-1);

    if (a->code > b->code) return ( 1);
    if (a->code < b->code) return (-1);

    if (a->nodeId > b->nodeId) return ( 1);
    if (a->nodeId < b->nodeId) return (-1);

    return (0);			/* equal */
}



/*
 * Build stats for local nodes only
 *
 * max... and min... are global variables
 */

void BuildLocalStats (RecordElement *reP)
{
    if (fabs (reP->Tdiff) > maxSkewPage)
	maxSkewPage = fabs (reP->Tdiff);
    
    if (IsInfInaccF (&reP->Iserv))
    {
	maxInaccPage = reP->Iserv;
    } else {
	if (reP->Iserv > maxInaccPage)
	    maxInaccPage = reP->Iserv;
    }
}

/*
 * Build stats for local and remote nodes
 */

void BuildNodeStats (RecordElement *reP, NodeElement *neP)
{
    double tempSkew, tempAbsSkew, tempInacc;

    /*
     * Update the statistics for this node
     */
    tempSkew = reP->Tdiff / 1e4;         /* make milliseconds */
    tempAbsSkew = fabs (tempSkew);
    /* 
     * If we're not using this node's stats yet, see if it's time to 
     * start using them
     */
    if ( ! neP->useStats)
    {
	/*
	 * If this is first record for this node (neP->seenFirst == 0), or
	 * if this is not an inflection (upturn after positive skew,
	 * downturn after neg), init lastSkew so we can tell if the 
	 * next record represents an inflection.
	 * If this is an inflection, then this node is no longer 
	 * adjusting to compensate for an initial skew, so start 
	 * collecting its stats.
	 * If allStats is set (-a switch on command line), then 
	 * FindNode() will set useStats when it mallocs this node's 
	 * NodeElement.
	 */
	if ((neP->seenFirst == 0) ||
	    ((tempAbsSkew <= fabs (neP->lastSkew)) &&
	     (SIGN (tempSkew) == SIGN (neP->lastSkew))) )
	{
	    neP->lastSkew = tempSkew;
	    neP->seenFirst = 1;
	} else {
	    neP->useStats = 1;
	}
    }
    /* 
     * Remember whether or not this record was used in stats for the 
     * display.  If it was, use it in the stats
     */
    if ( (reP->includeInStats = neP->useStats) )
    {
	/*
	 * Use signed skew for variance and std dev 
	 */
	neP->sumSkews   += tempSkew;
	neP->sumSkews2  += tempSkew * tempSkew;
	/*
	 * Use abs value of skew to calculate max, but store signed value
	 */
	if (fabs(neP->maxSkews) < tempAbsSkew)
	    neP->maxSkews = tempSkew;
	if (! IsInfInaccF (&reP->Iserv) )
	{
	    tempInacc = reP->Iserv / 1e4;         /* make milliseconds */
	    neP->sumInaccs  += tempInacc;
	    neP->sumInaccs2 += tempInacc * tempInacc;
	    if (neP->maxInaccs < tempInacc)
		neP->maxInaccs = tempInacc;
	}
	neP->count++;
    }
}



/* 
 * Update the global variables minTime and maxTime.  These are used to 
 * compute the vertical scale ( in routine LabelY() ).
 */

static void UpdateMinMaxTime (RecordElement *reP)
{
    double maxTemp, minTemp;

    /*
     * If the inaccuracy is infinite, don't try to scale to it,
     * just use the scalar time.
     * Otherwise, calculate the top and bottom of the inaccuracy
     * line this record will create into minTemp and maxTemp.
     */
    if (IsInfInaccF (&reP->Iserv))
    {
	maxTemp = minTemp = reP->Tdiff;
    } else {
	maxTemp = reP->Tdiff + reP->Iserv;
	minTemp = reP->Tdiff - reP->Iserv;
    }
    if ((maxTime < maxTemp) &&
	((maxVerticalScale == 0) ||
	 (fabs(maxTemp) <= maxVerticalScale)) )
	maxTime = maxTemp;
    
    if ((minTime > minTemp) &&
	((maxVerticalScale == 0) ||
	 (fabs(minTemp) <= maxVerticalScale)) )
	minTime = minTemp;
}



void ZeroStats (NodeElement *neP)
{
    neP->sumSkews = 0;
    neP->sumSkews2 = 0;
    neP->maxSkews = 0;
    neP->sumInaccs = 0;
    neP->sumInaccs2 = 0;
    neP->maxInaccs = 0;
    neP->lastSkew = 0;
    neP->count = 0;
}



static int FloatToUTC (utc_t *utcp, double *utcfp)
{
    timespec_t timevalue;

    timevalue.tv_sec = floor((*utcfp - K_BASE_TIME_DIFF_F) / 1e7 );
    timevalue.tv_nsec = (((*utcfp - K_BASE_TIME_DIFF_F) -
                          (timevalue.tv_sec * 1.e7)) * 1.e9);

    return(utc_mkbintime(utcp, &timevalue, (timespec_t *)NULL, 0L));
}



/*
 * returns EOF if end of file, else EOF + 1.
 */
static int ReadInputLine (RecordElement *reP)
{
    static int tpNum;
    int count;
    char nodeName[MAXNAME];
    char in_line[MAXLINE];

    /*
     * Read in the whole line and then scan it, to be sure that
     * scanf doesn't read from the wrong record if records are an
     * unexpected length.
     */
    do {
	if (NULL == fgets (in_line, MAXLINE, infile))
	    return (EOF);
	
	count = sscanf (in_line,
			"%ld %ld %s %lf %lf %lf %ld %ld %lf %ld",
			&reP->synchSerialNum,
			&reP->code,
			nodeName,
			&reP->Tcomp,
			&reP->Tdiff,
			&reP->Iserv,
			&reP->fault,
			&reP->recordCount,
			&reP->Iservs,
			&reP->globalSet);
	
    } while ((in_line[0] < '0') || /* first char of line must be an ascii digit 0-9 */
	     (in_line[0] > '9') ||
	     (count < 8));	  /* ignore blank lines and comments */
    /*
     * if trace file is old and doesn't have the last two items,
     * zero them.
     */
    if (count < 10)
    {
	reP->Iservs = 0;
	reP->globalSet = 0;
    }
    
    /* 
     * If the input trace's version number is less than 3, the 
     * Tdiff field has the wrong sign. Flip it here if need be.
     */
    if (traceVersion < 3)
	reP->Tdiff *= -1;
    
    /* 
     * Calc tdf if we haven't already
     */
    if (tdf == NO_TDF)
    {
	utc_t utc;
	
	FloatToUTC (&utc, &reP->Tcomp);
	/* 
	 * Default the tdf if it still has the initial value.
	 * tdf and tznam are global variables.
	 */
	if (tdf == NO_TDF) {
	    if (utc_localzone (tznam, sizeof(tznam), &tdf, (int *)NULL,
                               &utc) < 0) {
	        (void) utc_gmtzone(tznam, sizeof(tznam), &tdf, (int *)NULL,
                                   &utc);
	    }
	    tdf /= 60;
	}
    }
    /*
     * Match this record's node name to names in nodeArray.
     */
    switch (reP->code)
    {
    case REMSRVCODE:
	/*
	 * Add request number to time-provider names.
	 */
	if (strcmp (nodeName, "Time-Provider") == 0)
	{
	    if (++tpNum > 1)
		sprintf (nodeName, "Time-Provider, request #%d",tpNum);
	}
	/*
	 * Find node name in array and assign node id to record.
	 * Incr numNodes if this is a new node name.
	 */
	FindNodeName (nodeName, &reP->nodeId, &numNodes);
	/*
	 * Mark node as local or global
	 */
	nodeArray[reP->nodeId].globalSet = reP->globalSet;
	break;
	
    case LOCNODCODE:
	strncpy (myNodeName, nodeName, MYNAMELEN);
	break;
	
    case SEPCODE:
	tpNum = 0;
	break;
    }
    return (EOF + 1);		/* anything but EOF */
}

/*
 * FindNodeName outputs node id for node name.
 * node id is -1 into node array, node id + 'A' is letter on graph
 */
void FindNodeName (char *nodeName, int  *nodeIdP, int  *numNodesP)
{
    char *cp;
    int i;
    NodeElement *neP;

    for (i = *numNodesP-1 ; i >= 0 ; i--)
    {
	if (0 == strcmp (nodeName, nodeArray[i].nameP) )
	{
	    *nodeIdP = i;
	    return;
	}
    }
    if (*numNodesP < MAXNODES)
    {
	if ((cp = (char *) malloc (1 + strlen (nodeName))) != 0)
	{
	    strcpy (cp, nodeName);
	    neP = &nodeArray[*numNodesP];
	    neP->nameP = cp;
	    neP->useStats = allStats;
	    neP->seenFirst = 0;
	    ZeroStats (neP);

	    *nodeIdP = *numNodesP;
	    (*numNodesP)++;
	}
    }
}



/*
 * Returns:
 *    Double scaleFactor = multiply Y values by scaleFactor to fit on page.
 *
 * Side Effects:
 *    Writes postscript program for Y-axis with labels and horizontal
 *    lines at the ticks.
 */

static void LabelY (double *scaleFactorP, double *xAxisPosP)
{
    double maxInterval;		/* maxTime - minTime, what we scale to */
    double scaleFactor;		/* derive pixel lengths of graph lines */
    double xAxisPos;
    double tens;
    double xPos, yPos, dtemp;
    double increment, f;
    long intemp;
  
    /* 
     * If user has forced the scale with the centering switch
     * (-c scale), then override the calculated scale with the 
     * user-supplied value.
     * If user types -c 0, then do autoscaling: set the centerScale 
     * value to the max absolute value of maxTime and minTime on first 
     * page.
     */
    if (autoCenter)
    {
	autoCenter = 0;
	centeringScale = fabs(minTime);
	if (centeringScale < maxTime)
	    centeringScale = maxTime;
    }
    if (centeringScale > 0)
    {
	maxTime = centeringScale;
	minTime = -1 * centeringScale;
    }
    /*
     * Calculate the maximum interval along the Y-axis.
     * This routine will not work for maxInterval less than 1,
     * as there is no need for this program.
     */
    maxInterval = maxTime - minTime;
    if (maxInterval == 0)
    {
	maxTime += MIN_VERT_SCALE;
	minTime -= MIN_VERT_SCALE;
	maxInterval = maxTime - minTime;
    }
    tens = 1;
    while ((maxInterval / tens) >= 1)
    {
	tens *= 10;
    }
    tens /= 100;
    intemp = maxInterval / tens / 10;
    switch ((int)intemp)
    {
	case 0:
	case 1: increment = tens / 2;  break;
	case 2:
	case 3: increment = tens * 1;  break;
	case 4:
	case 5:
	case 6: increment = tens * 2;  break;
	case 7:
	case 8:
	case 9: increment = tens * 5;  break;
	default:increment = tens * 1;  break;
    }
    if (increment <= 0)
	increment = 1;
    maxTime = ceil (maxTime / increment) * increment;
    minTime = ceil (fabs (minTime) / increment) * increment *
	                                         (minTime <= 0 ? -1 : 1);
    maxInterval = maxTime - minTime;
    scaleFactor = YPIXELS / maxInterval;
    xAxisPos = scaleFactor * fabs(minTime) + YMARGIN;

    /*
     * Set font to 9 point
     *
     * Draw the Y-axis
     */
    xPos = XMARGIN + YAXISWIDTH;
    fprintf (outfile, "%% Draw the y-axis and its labels\n");
    fprintf (outfile, "/Times-Roman findfont 9 scalefont setfont\n");
    fprintf (outfile, ".25 setlinewidth \n");
    /*
     * Draw the Y-axis labels at the increments calculated
     */
    dtemp = minTime;	/* milliseconds from 100ns */
    for (f=maxTime ; f >= dtemp ; f -= increment)
    {
	/*
	 * Print numbers right-justified in axis margin
	 */
	fprintf (outfile, "(%1.0f) dup stringwidth pop ", f/1e4);
	yPos = xAxisPos + (scaleFactor * f);
	xPos = XMARGIN + YAXISWIDTH - TICKSIZE;
	fprintf (outfile, "neg %1.2f add %1.2f moveto show\n",
		 (xPos - 2.0), (yPos - 3.0));
	/*
	 * Draw the tick
	 */
	xPos = XMARGIN + YAXISWIDTH;
	fprintf (outfile, "newpath %1.2f %1.2f moveto ", xPos, yPos);
	intemp = TICKSIZE;
	fprintf (outfile, "-%d 0 rlineto stroke\n", intemp);
	/*
	 * Draw the grid line
	 */
	fprintf (outfile, "newpath %1.2f %1.2f moveto ", xPos, yPos);
	intemp = XPIXELS;
	fprintf (outfile, "%d 0 rlineto\n", intemp);
	fprintf (outfile, "gsave [1 5] 1 setdash stroke grestore\n");
    }
    /*
     * The Y-axis vertical lines, left and right margins
     * and X-axis top and bottom to complete the box.
     * Draw one more to highlight the zero line. 
     */
    xPos = XMARGIN + YAXISWIDTH;
    yPos = YMARGIN;
    fprintf (outfile, "newpath %1.2f %1.2f moveto ", xPos, yPos);
    yPos = YPIXELS;
    fprintf (outfile, "0 %1.2f rlineto \n", yPos);
    xPos = XPIXELS;
    fprintf (outfile, "%1.2f 0 rlineto \n", xPos);
    fprintf (outfile, "0 -%1.2f rlineto \n", yPos);
    fprintf (outfile, "-%1.2f 0 rlineto stroke\n", xPos);
    xPos = XMARGIN + YAXISWIDTH;
    yPos = xAxisPos;
    fprintf (outfile, "newpath %1.2f %1.2f moveto ", xPos, yPos);
    xPos = XPIXELS;
    fprintf (outfile, "%1.2f 0 rlineto stroke\n", xPos);
    /*
     * Clean up
     */
    fprintf (outfile, "1 setlinewidth ");
    fprintf (outfile, "\n");

    *scaleFactorP = scaleFactor;
    *xAxisPosP = xAxisPos;
}



/*
 * Returns number of pages printed
 */
static int MakeSummaryPage (void)
{
    double xPos, yPos;
    int i;
    tm_t tm_time;
    char *s;
    NodeElement *neP;

    /*
     * This page is in portrait mode to make more room for node names, so
     * X and Y limits are swapped.
     */

    fprintf (outfile, "\n%% Write the summary page\n\n");

    /*
     * Put out the title, if user gave us one
     */
    fprintf (outfile, "/Times-Roman findfont 18 scalefont setfont\n");

    yPos = PAGEWIDTH - (YMARGIN * 2);
    if (titleP)
    {
	WriteCenteredString (titleP,
			     (PAGEHEIGHT / 2.0),
			     yPos);
	yPos -= 18;
    }
    WriteCenteredString ("Summary",
			 (PAGEHEIGHT / 2.0),
			 yPos);

    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYFONTSIZE);

    xPos = 2.0 * INCH;
    yPos -= SUMMARYFONTSIZE * 1;	        /* blank line */

    if (! statOnly)
    {
	if (usedInfInacc)
	{
	    yPos -= SUMMARYFONTSIZE * 2;
	    fprintf (outfile, "newpath %1.2f %1.2f moveto ",
		     xPos - (0.25 * INCH),
		     yPos + (1.50 * SUMMARYFONTSIZE));
	    fprintf (outfile, "0 %1.2f rlineto\n", (-2.0 * SUMMARYFONTSIZE));
	    fprintf (outfile, "gsave %s stroke grestore\n", INFINACCPS);
	    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	    fprintf (outfile, "(Dashed interval line means infinite ");
	    fprintf (outfile, "inaccuracy.)show\n");
	}
	if (usedFault)
	{
	    yPos -= SUMMARYFONTSIZE * 2;
	    fprintf (outfile, "newpath %1.2f %1.2f moveto ",
		     xPos - (0.25 * INCH),
		     yPos + SUMMARYFONTSIZE);
	    fprintf (outfile, "0 %1.2f rlineto\n", (-2.0 * SUMMARYFONTSIZE));
	    fprintf (outfile, "gsave %s 2 setlinewidth stroke grestore\n",
		     FAULTPS);
	    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	    fprintf (outfile,
		     "(Gray calculated inaccuracy means fault)show\n");
	    yPos -= SUMMARYFONTSIZE;
	    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	    fprintf (outfile,
		     "(detected in associated synchronization.)show\n");
	}
	if (! allStats)
	{
	    yPos -= SUMMARYFONTSIZE * 2;
	    fprintf (outfile, "%1.2f %1.2f moveto ",
		     xPos - (.25 * INCH), yPos);
	    fprintf (outfile, "gsave %1.1f setgray ", IGNSTATGRAY);
	    fprintf (outfile, "(A)dup stringwidth pop 2 div neg 0 rmoveto ");
	    fprintf (outfile, "show grestore\n");
	    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	    fprintf (outfile, "(Gray node id letters indicate records ");
	    fprintf (outfile, "not included in statistics.)show\n");
	}
	yPos -= SUMMARYFONTSIZE * 2;
	fprintf (outfile, "%1.2f %1.2f moveto ",
		 xPos - (.25 * INCH), yPos);
	fprintf (outfile, "(A)dup stringwidth pop 2 div neg 0 rmoveto show\n");
	fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	fprintf (outfile,
	        "(Upper case node id letters indicate global servers.)show\n");

	yPos -= SUMMARYFONTSIZE * 1;
	fprintf (outfile, "%1.2f %1.2f moveto ",
		 xPos - (.25 * INCH), yPos);
	fprintf (outfile, "(a)dup stringwidth pop 2 div neg 0 rmoveto show\n");
	fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	fprintf (outfile,
		 "(Lower case node id letters indicate local servers.)show\n");
	
	yPos -= SUMMARYFONTSIZE * 2;
	fprintf (outfile, "newpath %1.2f %1.2f moveto ",
		 xPos - (0.25 * INCH),
		 yPos + (1.50 * SUMMARYFONTSIZE));
	fprintf (outfile, "0 %1.2f rlineto\n", (-1.5 * SUMMARYFONTSIZE));
	fprintf (outfile, "gsave 2.0 setlinewidth stroke grestore\n");
	fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	fprintf (outfile, "(Thick interval line shows inaccuracy server ");
	fprintf (outfile, "reported.)show\n");
	
	yPos -= SUMMARYFONTSIZE * 1;
	fprintf (outfile, "newpath %1.2f %1.2f moveto ",
		 xPos - (0.25 * INCH),
		 yPos + (1.50 * SUMMARYFONTSIZE));
	fprintf (outfile, "0 %1.2f rlineto\n", (-3.0 * SUMMARYFONTSIZE));
	fprintf (outfile, "gsave 1.0 setlinewidth stroke grestore\n");
	fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	fprintf (outfile, "(Thin interval line shows inaccuracy added ");
	fprintf (outfile, "for round-trip time & clerk drift)show\n");

	yPos -= SUMMARYFONTSIZE * 1;
	fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
	fprintf (outfile, "((on calculated time shows added inaccuracy ");
	fprintf (outfile, "for pre-adjustment skew).)show\n");
    }

    yPos -= SUMMARYFONTSIZE * 2;

    tm_time = *ConvertFloatToTm (firstTime);
    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
    fprintf (outfile, "(Start:)show\n");
    fprintf (outfile, "%1.2f %1.2f moveto ", xPos + 30, yPos);
    fprintf (outfile, "(%d-%d-%d-%d:%02d   TDF=%d %s)show\n",
	     tm_time.tm_year + 1900,
	     tm_time.tm_mon + 1,
	     tm_time.tm_mday,
	     tm_time.tm_hour,
	     tm_time.tm_min,
	     tdf,
	     tznam);

    tm_time = *ConvertFloatToTm (lastTime);
    fprintf (outfile, "%1.2f %1.2f moveto ", xPos + (3 * INCH), yPos);
    fprintf (outfile, "(End:)show\n");
    fprintf (outfile, "%1.2f %1.2f moveto ", xPos + (3 * INCH) + 30, yPos);
    fprintf (outfile, "(%d-%d-%d-%d:%02d)show\n",
	     tm_time.tm_year + 1900,
	     tm_time.tm_mon + 1,
	     tm_time.tm_mday,
	     tm_time.tm_hour,
	     tm_time.tm_min);

    yPos -= SUMMARYFONTSIZE * 1.5;

    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
    fprintf (outfile, "(Input trace is version %d.)show\n", traceVersion);
    
    yPos -= SUMMARYFONTSIZE * 2;

    /*
     * Now write out the node id summary and statistics
     */
    xPos = SUMMARYTAB3;
    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYSTATSIZE);

    if (allStats)
	s = "Statistics gathered since beginning of trace.";
    else
	s = "Statistics gathered since first inflection in skew for each node.";
    WriteCenteredString (s,
			 xPos + (STATTABSIZE * 4.0),
			 yPos);

    yPos -= SUMMARYFONTSIZE * 1.5;

    WriteCenteredString ("Skews to Calculated Time (ms)",
			 xPos + (STATTABSIZE * 2.5),
			 yPos);
    WriteCenteredString ("Finite Inaccuracies (ms)",
			 xPos + (STATTABSIZE * 5.5),
			 yPos);

    yPos -= SUMMARYFONTSIZE * 1;

    WriteCenteredString ("Samples",
			 xPos + (STATTABSIZE * 0.5),
			 yPos);
    WriteCenteredString ("Mean",
			 xPos + (STATTABSIZE * 1.5),
			 yPos);
    WriteCenteredString ("Std Dev",
			 xPos + (STATTABSIZE * 2.5),
			 yPos);
    WriteCenteredString ("Max",
			 xPos + (STATTABSIZE * 3.5),
			 yPos);
    WriteCenteredString ("Mean",
			 xPos + (STATTABSIZE * 4.5),
			 yPos);
    WriteCenteredString ("Std Dev",
			 xPos + (STATTABSIZE * 5.5),
			 yPos);
    WriteCenteredString ("Max",
			 xPos + (STATTABSIZE * 6.5),
			 yPos);
    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYFONTSIZE);

    yPos -= SUMMARYFONTSIZE * 1;

    WriteSummaryLine ('@', &myNodeElement, yPos);
    yPos -= SUMMARYFONTSIZE;
    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYSTATSIZE);
    fprintf (outfile, "%1.2f %1.2f moveto ", SUMMARYTAB2, yPos);
    fprintf (outfile, "(%s @ this node made this trace)show\n", variant);
    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYFONTSIZE);

    yPos -= SUMMARYFONTSIZE * 0.5;

    neP = &nodeArray[0];
    for (i=0 ; i<numNodes ; i++)
    {
	yPos -= SUMMARYFONTSIZE * 1.25 * 2;
	WriteSummaryLine (i + (neP->globalSet ? 'A' : 'a'),
			  neP,
			  yPos);
	neP++;
    }
    /*
     * Return number of pages printed
     */
    return (1);
}



static void WriteSummaryLine (int c, NodeElement *neP, double yPos)
{
#define NSLEN 80
    int result;
    NodeAddr nodeAddr;
    double mean, variance, stddev;
    char ns[NSLEN];
    int intArray[6];
    int i;

    /*
     * pass word-aligned ints for the MIPS chip
     */
    result = sscanf (neP->nameP, "%x-%x-%x-%x-%x-%x",
		     &intArray[0],
		     &intArray[1],
		     &intArray[2],
		     &intArray[3],
		     &intArray[4],
		     &intArray[5]);
    /*
     * If the node node was an ethernet address, break it out,
     * else just use the name as is.
     */
    if (result == K_ETHERNET_ADDR_SIZE)
    {
	/*
	 * Move the ints into the ethernet addr's byte array
	 */
	for (i=0 ; i<6 ; i++)
	{
	    nodeAddr.ethernetAddr.bytes[i] = intArray[i];
	}
	/* 
	 * Find the node name
	 */
	SprintNodeName(ns, &nodeAddr, NSLEN, 0);
    } else
	strncpy (ns, neP->nameP, NSLEN);

    fprintf (outfile, "%1.2f %1.2f moveto ", SUMMARYTAB0, yPos+SUMMARYFONTSIZE * 1.25);
    fprintf (outfile, "(%c)show\n", c);
    fprintf (outfile, "%1.2f %1.2f moveto ", SUMMARYTAB1, yPos+SUMMARYFONTSIZE * 1.25);
    fprintf (outfile, "(=)show\n");
    fprintf (outfile, "%1.2f %1.2f moveto ", SUMMARYTAB2, yPos+SUMMARYFONTSIZE * 1.25);
    fprintf (outfile, "(%s)show\n", ns);

    /*
     * If we still haven't seen an inflection for this node (or don't
     * need one), zero the stats.
     */
    if ( ! neP->useStats)
	ZeroStats (neP);

    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYSTATSIZE);

    fprintf (outfile, "%1.2f %1.2f moveto (%ld)dup\n",
	     SUMMARYTAB3 + (STATTABSIZE * 0.75),
	     yPos,
	     neP->count);
    fprintf (outfile, " stringwidth pop neg 0 rmoveto show\n");

    mean = neP->count ? neP->sumSkews / neP->count : 0;
    WriteRightedDouble (mean,
			SUMMARYTAB3 + (STATTABSIZE * 2.0),
			yPos);

    variance = neP->count ? (neP->sumSkews2 / neP->count) - (mean * mean) : 0;
    stddev = sqrt (variance);
    WriteRightedDouble (stddev,
			SUMMARYTAB3 + (STATTABSIZE * 3.0),
			yPos);

    WriteRightedDouble (neP->maxSkews,
			SUMMARYTAB3 + (STATTABSIZE * 4.0),
			yPos);

    mean = neP->count ? neP->sumInaccs / neP->count : 0;
    WriteRightedDouble (mean,
			SUMMARYTAB3 + (STATTABSIZE * 5.0),
			yPos);

    variance = neP->count ? (neP->sumInaccs2 / neP->count) - (mean * mean) : 0;
    stddev = sqrt (variance);
    WriteRightedDouble (stddev,
			SUMMARYTAB3 + (STATTABSIZE * 6.0),
			yPos);

    WriteRightedDouble (neP->maxInaccs,
			SUMMARYTAB3 + (STATTABSIZE * 7.0),
			yPos);

    fprintf (outfile, "/Times-Roman findfont %d scalefont setfont\n",
	     SUMMARYFONTSIZE);
}


/*
 * Called just after opening the trace file for input.
 * Exepects to find the version number in the first line of the trace file.
 *
 * Fills in global variable traceVersion with version number found at
 * top of input file. traceVersion set to zero if no version # found.
 *
 * Also fills the global string variant with "server" or "clerk",
 * copied from the input file.
 */
static void ReadTraceVersion (void)
{
    char in_line[MAXLINE];
    
    variant[0] = '\0';
    
    if (NULL == fgets (in_line, MAXLINE, infile))
	traceVersion = 0;
    else
    {
	if (2 != sscanf (in_line, "Version %d Variant %s",
			 &traceVersion,
			 variant))
	{
	    traceVersion = 4;
	    (void) fprintf (stderr,
		"DTSS-I-GRAPH - First line is not a Version/Variant line\n"
			    );
	}
	
    }
}


/*
 * Render the 64-bit computed time into a form that
 *  calls can deal with it.
 *
 * tdf is a global variable: minutes of timezone differential from UTC.
 *
 * Returns a pointer to a tm_t struct. Note that the tm_t struct is
 * overwritten on the next call to localtime, so caller should use or
 * copy the result before any other calls to localtime can happen.
 */

static tm_t *ConvertFloatToTm (double ftime)
{
    time_t secs;
    
    secs = ( (ftime - K_BASE_TIME_DIFF_F) / 1e7 ) + (tdf * 60);
    return ((tm_t *)localtime (&secs));
}


void WriteCenteredString (char *s, double xPos, double yPos)
{
    fprintf (outfile, "%1.2f %1.2f moveto (%s)dup\n", xPos, yPos, s);
    fprintf (outfile, " stringwidth pop 2 div neg 0 rmoveto show\n");
}



void WriteRightedDouble (double n, double xPos, double yPos)
{
    fprintf (outfile, "%1.2f %1.2f moveto ", xPos, yPos);
    if (n > 1e6)
	fprintf (outfile, "(%1.6g)dup\n", n);
    else
	fprintf (outfile, "(%1.2f)dup\n", n);
    fprintf (outfile, " stringwidth pop neg 0 rmoveto show\n");
}



/*
 *++
 *  SprintNodeName()
 *
 *  Functional Description:
 *
 *  Convert a 48 bit ethernet address into the form:
 *
 *  00x-00x-00x-00x-00x (nodename)
 *
 *
 *  
 *  Inputs:
 *
 *    nodeAddr - addr of NodeAddr struct
 *    strsize  - max size of output string
 *    failMode - if the address can not be translated, then print the
 *               in either area.number or number form.
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    nameText - output string;
 *
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *    values returned from netacp qio.
 *
 *  Side Effects:
 *    None.
 *
 *--
 */
void SprintNodeName(  char *nameText,   NodeAddr *nodeAddrPtr,
  int  len,  int  failMode)
{

    unsigned short decnetAddr;
    unsigned char *ethernetAddrPtr;
    char nameTextTemp[NSLEN];

    ethernetAddrPtr = nodeAddrPtr->ethernetAddr.bytes;
    decnetAddr = ((unsigned int)((nodeAddrPtr->decnet48.decnetAddr)[1]) << 8) +
		 (unsigned int)((nodeAddrPtr->decnet48.decnetAddr)[0]);

    (void) memset(nameTextTemp, 0, sizeof(nameTextTemp));
    (void) sprintf(nameTextTemp,
		   "%02X-%02X-%02X-%02X-%02X-%02X",
		   ethernetAddrPtr[0],
		   ethernetAddrPtr[1],
		   ethernetAddrPtr[2],
		   ethernetAddrPtr[3],
		   ethernetAddrPtr[4],
		   ethernetAddrPtr[5]);

    /*
     * If this is a phase IV decnet address, then append the node addr
     */
    if (failMode == 0)        /* aa.aaaa */
    {
	if ((ethernetAddrPtr[0] == 0xaa) &&
	    (ethernetAddrPtr[1] == 0x00) &&
	    (ethernetAddrPtr[2] == 0x04) &&
	    (ethernetAddrPtr[3] == 0x00)) 
	{
	    sprintf(&nameTextTemp[strlen(nameTextTemp)]," (%d.%d)",
		    decnetAddr / 1024,
		    decnetAddr % 1024);
	}
    }
    strncpy (nameText, nameTextTemp, 0);
}
/* End of routine SprintNodeName */
