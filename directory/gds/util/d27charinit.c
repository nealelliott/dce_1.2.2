/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27charinit.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:16:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:15  root]
 * 
 * Revision 1.1.2.4  1994/07/15  13:29:31  marrek
 * 	Remove empty comment line.
 * 	[1994/07/15  13:00:12  marrek]
 * 
 * Revision 1.1.2.3  1994/07/07  07:02:43  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:58:57  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:46  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:14  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:04  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:57  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:47  marrek]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char d27charinit_rcsid[] = "@(#)$RCSfile: d27charinit.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:36 $";
#endif

/*module***********************************************************************
 *
 * PRODUCT NAME / SUBSYSTEM NAME:
 *    OCIS / ORGMAN
 * MODULE NAME / VERSION:
 *    omCharInit.c (%M%) / %I%
 * DATE / LAST MODIFICATION:
 *    %D% / %E%
 * DESCRIPTION:
 *    Initialization for omChar functions.
 * OFFERED FUNCTIONS:
 *    OmCharInit
 *    OmCharReadFile
 * OFFERED DATA:
 *    -
 * AUTHOR:
 *    Fiedrich, AP 154
 *    vers.|   date   |                changes                  | KZ | ER#/CR#
 *    -------------------------------------------------------------------------
 *    1.00 | 10.09.92 | initial state                           | Fd |
 *    1.01 | 30.10.92 | array omCharEqual introduced            | Fd | internal
 *    1.02 | 05.04.93 | ISO8859/T.61 conversion deactivation    | Fd | internal
 *    1.03 | 19.05.93 | changes due to shell variable concept   | Fd | internal
 *
 *headerend*******************************************************************/

/*****************/
/* include files */
/*****************/

#include 	<gds.h>
#include	<dce/d27util.h>

#include <d27char.h>
#include <d27chardef.h>

/**********************/
/* defines 	      */
/**********************/

#define D27_CONV_PATH_MAX 1023

/**********************/
/* external variables */
/**********************/

extern int omCharConvSwitch;           /* conversion switch */
extern unsigned char omCharVal[];      /* valid codes table */
extern unsigned char omCharExc[][3];   /* transformation exception table */
extern unsigned char omCharConv1[][2]; /* conversion table 1 */
extern unsigned char omCharConv2[];    /* conversion table 2 */
extern int omCharInd[];                /* index table */
extern unsigned char omCharConv3[][2]; /* conversion table 3 */
extern char omCharEqual[];             /* equality table */
extern unsigned char omCharPrs[][2];   /* substitution table */
extern char *omCharErrC[];             /* error messages */

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharInit
 * DESCRIPTION:
 *    Reads in the conversion files, if present,
 *    and sets the valid code and conversion tables.
 * CALL SYNTAX:
 *    void OmCharInit(void)
 * PARAMETERS:
 *    -
 *
 *headerend*******************************************************************/

void OmCharInit(void)
{  int i, j, k;                         /* counters */
   char cfname[D27_CONV_PATH_MAX];      /* complete name of conversion file */
   int rootlen;                         /* length of root pathname */
   unsigned char mask;                  /* bit mask */

   /*************************/
   /* read conversion files */
   /*************************/

   (void) sprintf(cfname, "%s%s", dcelocal_path, D27_CLIENT_DIR);

					/* get T.61 conversion file name */
   rootlen = strlen(cfname);
   (void) strcat(cfname, OmCHAR_FILE_T61);
   (void) OmCharReadFile(cfname, OmCHAR_SEL_T61);
					/* read T.61 conversion file */

   cfname[rootlen] = '\0';              /* get PRS conversion file name */
   (void) strcat(cfname, OmCHAR_FILE_PRS);
   (void) OmCharReadFile(cfname, OmCHAR_SEL_PRS);
					/* read Printable String conv. file */

   omCharConvSwitch = TRUE;          /* set conversion switch to TRUE */

   /***************************************/
   /* write upper half of array omCharVal */
   /***************************************/

   for (i = 0x80; i < 256; i++)         /* clear all elements */
      omCharVal[i] = 0;
   for (i = 0; i < 96; i++)             /* copy values from omCharConv1 to */
					/* omCharVal array */
   {  if ((unsigned int) omCharConv1[i][0] == 1)
					/* if only ISO 8859 code exists */
      {  omCharVal[i + 0xa0] = (unsigned char) CHS7;
					/* set omCharVal element */
	 omCharConv1[i][0] = (unsigned char) 0;
					/* set omCharConv1 element */
      }
      else if (omCharConv1[i][0] > (unsigned char) 1)
					/* if ISO 8859 and T.61 codes exist */
	 omCharVal[i + 0xa0] = (unsigned char) (CHS7 + CHS8);
   }                                    /* set omCharVal element */

   /******************************/
   /* fill in conversion table 2 */
   /******************************/

   for (i = 0; i < 96; i++)             /* for all indexes */
   {  if (omCharConv1[i][0] >= (unsigned char) 0xa0)  
      {  if (    omCharConv1[i][0] < (unsigned char) 0xc0
	      || omCharConv1[i][0] > (unsigned char) 0xcf)
					/* if T.61 code in range for 1 byte */
	    omCharConv2[(int) omCharConv1[i][0] - 0xa0] =
	       (unsigned char) (i + 0xa0); /* set omCharConv2 element */
	 else                              /* else */
	    omCharConv2[(int) omCharConv1[i][0] - 0xa0] = (unsigned char) 0;
					/* set omCharConv2 element */
      }
   }

   /**********************************************/
   /* fill in index table and conversion table 3 */
   /**********************************************/

   j = 0;                               /* start. index in array omCharConv3 */
   for (k = 0; k < 16; k++)             /* index in array omCharInd */
   {  omCharInd[k] = j;
      for (i = 0; i < 96; i++)          /* index in array omCharConv1 */
      {  if (omCharConv1[i][0] == (unsigned char) (0xc0 + k))
	 {  omCharConv3[j][0] = omCharConv1[i][1];
	    omCharConv3[j++][1] = (unsigned char) (i + 0xa0);
	 }
      }
   }
   omCharInd[k] = j;

   /**************************/
   /* fill in equality table */
   /**************************/

   mask = CHS7 + CHS8;                  /* fill in lower half */
   for (i = 0; i < 128; i++)
      omCharEqual[i] = ((omCharVal[i] & mask) == mask) ? '1' : '0';
   for (i = 0; (int) omCharExc[i][0] != 0; i++)
					/* correct exception entries */
      omCharEqual[(int) omCharExc[i][0]] = '0';
   for (i = 128; i < 0xa0; i++)         /* erase no-character area */
      omCharEqual[i] = '0';
   for (i = 0; i < 96; i++)             /* fill in upper half */
      omCharEqual[i + 0xa0] = (((i + 0xa0) == (int) omCharConv1[i][0])
	 && ((int) omCharConv1[i][1] == 0)) ? '1' : '0';
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharReadFile
 * DESCRIPTION:
 *    Reads the T.61 or Printable String conversion file.
 * CALL SYNTAX:
 *    int OmCharReadFile(char *file, OmCharSelFile sel)
 * PARAMETERS:
 *    IN:    file   file name
 *    IN:    sel    conversion file selector
 *    OUT:   return TRUE = file could be opened, FALSE = failure
 *
 *headerend*******************************************************************/

int OmCharReadFile(char *file, OmCharSelFile sel)
{
   FILE *dp;                              /* conversion file */
   int i;                                 /* counter */
   int triple[3];                         /* conversion triple */
   int value;                             /* symbol value */
   int end;                               /* end flag */
   int line;                              /* line in file */

   /************************/
   /* open conversion file */
   /************************/

   if ((dp = fopen(file, "r")) == (FILE *) NULL)
					/* open file - if not existent */
      return(FALSE);                    /* return */

   /**************************/
   /* erase conversion table */
   /**************************/

   if (sel == OmCHAR_SEL_T61)           /* if T.61 conversion file */
   {  for (i = 0; i < 96; i++)          /* for all indexes */
      {  omCharConv1[i][0] = 0;         /* erase first element */
	 omCharConv1[i][1] = 0;         /* erase second element */
      }
   }
   else                                 /* else (if Printable String file) */
   {  for (i = 0; i < 96; i++)          /* for all indexes */
      {  omCharPrs[i][0] = 0;           /* erase first element */
	 omCharPrs[i][1] = 0;           /* erase second element */
      }
   }
   line = 1;                            /* initial value of line number */

   /****************************/
   /* fill in conversion table */
   /****************************/

   for (;;)                             /* for all conversions in file */
   {  for (i = 0; i < 3; i++)           /* clear conversion triple */
	 triple[i] = 0;
      end = FALSE;                      /* reset end flag */
      for (i = 0; ; i ++)               /* for all symbols in conversion */
      {  value = OmCharReadSymbol(dp);  /* read symbol */
	 if (i == 0 && value == OmCHAR_EOF)
					/* if immediately end of file */
	 {  end = TRUE;                 /* set end flag */
	    break;                      /* leave loop */
	 }
	 else if (value == OmCHAR_EOC || value == OmCHAR_EOF)
					/* else if end of conversion */
	    break;                      /* leave loop */
	 else if (i < 3 && value == OmCHAR_INV)
					/* else if invalid symbol */
	 {  
	    triple[0] = 0;              /* mark first element of triple */
	 }
	 else if (i < 3)                /* else if count not exceeded */
	    triple[i] = value;          /* store number in triple */
      }
      if (end == TRUE)                  /* if end of file */
	 break;                         /* leave loop */
      if (triple[0] == 0)               /* if empty triple */
      {  line++;                        /* increment line number */
	 continue;                      /* go to next conversion */
      }
      if (triple[0] >= 0xa0 && triple[0] <= 0xff)
					/* if element 0 in range */
      {  if (sel == OmCHAR_SEL_T61)     /* if T.61 conversion file */
	 {  if (triple[1] == 0)         /* if element 1 not present */
	       omCharConv1[triple[0] - 0xa0][0] = 1;
					/* mark omCharConv1 element */
	    else if (   triple[1] >= 0xa0 
		     && (triple[1] < 0xc0 || triple[1] > 0xcf)
		     && triple[2] == 0
		    )                   /* else if valid 1 byte T.61 code */
	       omCharConv1[triple[0] - 0xa0][0] = (unsigned int) triple[1];
					/* enter number in omCharConv1 */
	    else if (   triple[1] >= 0xc0
		     && triple[1] <= 0xcf
		     && triple[2] <= 0x7f
		     && triple[2] != 0
		    )                   /* else if valid 2 byte T.61 code */
	    {  omCharConv1[triple[0] - 0xa0][0] = (unsigned char) triple[1];
					/* enter numbers in omCharConv1 */
	       omCharConv1[triple[0] - 0xa0][1] = (unsigned char) triple[2];
	    }
	 }
	 else                           /* else (if Printable String conv.) */
	 {  if (   (triple[1] <= 0x7f)
		&& ( ( (unsigned int) omCharVal[triple[1]] & CHS5) > 0)
		&& (triple[2] <= 0x7f)
		&& ( (triple[2] == 0) ||
		   ( ( (unsigned int) omCharVal[triple[2]] & CHS5) > 0)))
					/* if valid conversion */
	    {  omCharPrs[triple[0] - 0xa0][0] = (unsigned char) triple[1];
					/* write first byte to table */
	       omCharPrs[triple[0] - 0xa0][1] = (unsigned char) triple[2];
	    }                           /* write second byte to table */
	 }
      }

      line++;                           /* increment line number */
   }

   /*******/
   /* end */
   /*******/

   (void) fclose(dp);                   /* close conversion file */
   return(TRUE);
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharReadSymbol
 * DESCRIPTION:
 *    Reads the next symbol in the conversion file.
 * CALL SYNTAX:
 *    int OmCharReadSymbol(FILE *dp)
 * PARAMETERS:
 *    IN:     dp      pointer to conversion file
 *    OUT     return  error code or number read
 *
 *headerend*******************************************************************/

int OmCharReadSymbol(FILE *dp)
{   int value;                  /* number value */
    int error;                  /* error indicator */
    int c;                      /* character read from file */

   /************************************/
   /* get to first non-blank character */
   /************************************/

   while ((c = getc(dp)) == (int) OmCHAR_BLK)
      ;                         /* skip leading blanks */

   /***************/
   /* end of file */
   /***************/

   if (c == EOF)                /* if end of file found */
      return(OmCHAR_EOF);       /* return */

   /*********************/
   /* end of conversion */
   /*********************/
 
   if (c == (int) OmCHAR_NLN)   /* if newline character found */
      return(OmCHAR_EOC);       /* return */

   if (c == (int) OmCHAR_COM)   /* if start of comment character found */
   {  for (;;)                  /* forever */
      {  c = getc(dp);          /* read character */
	 if (c == (int) OmCHAR_NLN)
				/* if newline character found */
	    return(OmCHAR_EOC); /* return */
	 if (c == EOF)          /* if end of file found */
	    return(OmCHAR_EOF); /* return */
      }
   }

   /*****************/
   /* other symbols */
   /*****************/

   value = 0;                   /* initial number value */
   error = 0;                   /* initial error value */
   for (;;)                     /* forever */
   {  if (c >= (int) '0' && c <= (int) '9') /* add digit to value */
	 value = value * 16 + c - (int) '0';
      else if (c >= (int) 'a' && c <= (int) 'f')
	 value = value * 16 + c - (int) 'a' + 10;
      else if (c >= (int) 'A' && c <= (int) 'F')
	 value = value * 16 + c - (int) 'A' + 10;
      else                      /* if invalid digit */
      {  value = 0;             /* set value to 0 */
	 error = 1;             /* set error indicator */
      }

      if (value > 255)          /* if value exceeds limit */
      {   value = 0;            /* set value to 0 */
	  error = 1;            /* set error indicator */
      }

      c = getc(dp);             /* read next character */
      if (   c == (int) OmCHAR_BLK || c == (int) OmCHAR_COM 
	  || c == (int) OmCHAR_NLN || c == EOF)
				/* if separator character */
      {  (void) ungetc(c, dp);  /* push back on input buffer */
	 break;                 /* leave loop */
      }
   }
   if (error == 0)              /* if error indicator not set */
      return(value);            /* return */
   else                         /* else */
      return(OmCHAR_INV);       /* return */
}
