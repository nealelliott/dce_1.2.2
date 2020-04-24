/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: d27charconv.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:15:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:12  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:48  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:16  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:44  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:12  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:03  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:09:53  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:43  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27charconv.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:34 $";
#endif

/*module***********************************************************************
 *
 * PRODUCT NAME / SUBSYSTEM NAME:
 *    OCIS / ORGMAN
 * MODULE NAME / VERSION:
 *    omCharConv.c (%M%) / %I%
 * DATE / LAST MODIFICATION:
 *    %D% / %E%
 * DESCRIPTION:
 *    Character set test and conversion functions.
 * OFFERED FUNCTIONS:
 *    OmCharRemoveBlanks
 *    OmCharValidCodes
 *    OmCharT61Length
 *    OmCharAdaptPrs
 *    OmCharIsoT61
 *    OmCharT61Iso
 * OFFERED DATA:
 *    -
 * AUTHOR:
 *    Fiedrich, AP 154
 * HISTORY:
 *    vers.|   date   |                changes                  | KZ | ER#/CR#
 *    -------------------------------------------------------------------------
 *    1.00 | 10.09.92 | initial state                           | Fd |         
 *    1.01 | 17.09.92 | OmCharValidCodes: HTAB introduced       | Fd | internal
 *    1.02 | 28.09.92 | new definition OmCHAR_ASCII_NOT_COLON   | Fd | A0158768
 *    1.03 | 13.10.92 | new character set OmCHAR_ASCII_NOT_COSE | Fd | internal
 *    1.04 | 04.11.92 | array omCharEqual introduced            | Fd | internal
 *    1.05 | 05.04.93 | ISO8859/T.61 conversion deactivation    | Fd | internal
 *    1.06 | 28.04.93 | new character sets                      | Fd | internal
 *    1.10 | 03.05.93 | character '-' allowed                   | Fd | internal
 *
 *headerend*******************************************************************/

/*****************/
/* include files */
/*****************/

#include <d27char.h>
#include <d27chardef.h>

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
extern char omCharEqual[];             /* equality array */
extern unsigned char omCharPrs[][2];   /* substitution table */

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharRemoveBlanks
 * DESCRIPTION:
 *    Removes superfluous blanks in a string.
 *    (Execution time is optimized for non-superfluous-blanks/non-DN strings.)
 * CALL SYNTAX:
 *    int OmCharRemoveBlanks(int is_dn, const char *string1, char *string2)
 * PARAMETERS:
 *    IN:     is_dn   attribute class (TRUE = DN, FALSE = non-DN)
 *    IN:     string1 input string (terminated by '\0')
 *    OUT:    string2 copy of input string with superfluous blanks removed
 *                    (terminated by '\0')
 *    OUT:    return  statement whether string2 differs from string1
 *                    (TRUE = different, FALSE = equal)
 *
 *headerend*******************************************************************/

int OmCharRemoveBlanks(int is_dn, const char *string1, char *string2)
{  int i, j;                            /* counters */

   if (is_dn == FALSE)                  /* if non-DN string */
   {  for (i = 0; *(string1 + i) != '\0'; i++)
					/* get to first non-blank character */
      {  if (*(string1 + i) != OmCHAR_BLK)
	    break;
      }
      for (j = 0; *(string1 + i) != '\0'; i++, j++)
					/* copy input to output string */
	  *(string2 + j) = *(string1 + i);
      for (j--; j >= 0; j--)            /* get to last non-blank character */
      {  if (*(string2 + j) != OmCHAR_BLK)
	    break;
      }
      *(string2 + ++j) = '\0';        /* terminate string2 */
   }
   else                                 /* else (if DN-string) */
   {  i = 0;                            /* starting index for input string */
      j = 0;                            /* starting index for output string */
      for (;;)                          /* eternal loop */
      {  for ( ; *(string1 + i) != '\0'; i++)
					/* get to first non-blank character */
	 {  if (*(string1 + i) != OmCHAR_BLK)
	       break;
	 }
	 if (*(string1 + i) == '\0')    /* if at input string termination */
	 {  *(string2 + j) = '\0';      /* terminate string2 */
	    break;                      /* leave loop */
	 }
	 else if (j != 0)               /* if not start of output string */
	    *(string2 + j++) = OmCHAR_BLK; /* write blank to output string */
	 if (*(string1 + i) != '\0')    /* copy partial string from input */
	 {  *(string2 + j++) = *(string1 + i++); /* to output */
	    for ( ; *(string1 + i) != '\0'; i++, j++)
					/* get to next non-blank character */
	    {  if (*(string1 + i) == OmCHAR_BLK)
		  break;
	       *(string2 + j) = *(string1 + i);
	    }
	    if (*(string1 + i) == '\0') /* if at string termination */
	    {  *(string2 + j) = '\0';   /* terminate string2 */
	       break;                   /* break */
	    }
	 }
      }
   }
   if (i != j)                          /* if strings unequal */
      return(TRUE);                     /* return TRUE */
   else                                 /* else */
      return(FALSE);                    /* return FALSE */
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharValidCodes
 * DESCRIPTION:
 *    Tests a string whether characters are included in given character set.
 * CALL SYNTAX:
 *    int OmCharValidCodes(OmCharCharSet charset, int nl_valid,
 *                        const char *string1, int length1,
 *                        char *string2, int *length2, int is_search)
 * PARAMETERS:
 *    IN:     charset   name of character set
 *    IN:     nl_valid  Newline additionally valid: TRUE = yes, FALSE = no.
 *    IN:     string1   input string (not terminated by '\0')
 *    IN:     length1   length of string1
 *    OUT:    string2   copy of input string with invalid characters replaced
 *                      by special character (not terminated by '\0')
 *    OUT:    length2   length of string2
 *    IN:     is_search TRUE = search value, FALSE = attribute value
 *    OUT:    return    number of invalid characters
 *
 *headerend*******************************************************************/

int OmCharValidCodes(OmCharCharSet charset, int nl_valid,
		     const char *string1, int length1,
		     char *string2, int *length2, int is_search)
{  int charmask;                        /* character set mask */
   int i, j;                            /* counters */
   int deval;                           /* devaluation flag */
   int devalold;                        /* old value of devaluation flag */
   char *invc;                          /* pointer to invalid codes string */
   char *invfc;                         /* pointer to first-position */
					/* invalid codes string */
   int invnu;                           /* number of invalid characters */

   /********************************************************/
   /* allocate character set mask and invalid codes string */
   /********************************************************/

   switch (charset)
   {  case OmCHAR_DIGIT:
	 charmask = CHS1;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_DIGIT_BLANK:
	 charmask = CHS2;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_DIGIT_LOWER_ALPHA:
	 charmask = CHS3;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_ALPHA:
	 charmask = CHS4;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_PRINTABLE:
	 charmask = CHS5;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_ASCII:
	 charmask = CHS6;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_ASCII_NOT_COLON:
	 charmask = CHS6;
	 invc = ":";
	 invfc = "";
	 break;
      case OmCHAR_ASCII_NOT_COSE:
	 charmask = CHS6;
	 invc = ":;";
	 invfc = "";
	 break;
      case OmCHAR_ASCII_V3:
	 charmask = CHS6;
	 invc = ":;,";
	 invfc = "";
	 break;
      case OmCHAR_ISO:
	 charmask = CHS7;
	 invc = "\t";
	 invfc = "";
	 break;
      case OmCHAR_ISO_NOT_SLASH:
	 charmask = CHS7;
	 invc = "\t/";
	 invfc = "";
	 break;
      case OmCHAR_ISO_V2:
	 charmask = CHS7;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_T61:
	 charmask = CHS8;
	 invc = "";
	 invfc = "";
	 break;
      case OmCHAR_T61_NOT_SEMICOLON:
	 charmask = CHS8;
	 invc = ";!";
	 invfc = "";
	 break;
      default:
	 charmask = 0xff;
	 invc = "";
	 invfc = "";
	 break;
   }

   /****************/
   /* empty string */
   /****************/

   if (length1 == 0)                    /* if empty string */
   {  *length2 = 0;                     /* set length2 */
      return(0);                        /* return */
   }

   /**************************/
   /* all characters allowed */
   /**************************/

   if (charmask == 0xff)                /* if all characters allowed */
   {  for (i = 0; i < length1; i++)     /* copy input to output string */
	 *(string2 + i) = *(string1 + i);
      *length2 = length1;               /* set output length */
      return(0);
   }

   /***************/
   /* other cases */
   /***************/

   invnu = 0;                           /* initial value nu. of inv. char. */
   deval = FALSE;                       /* starting value of deval. flag */

   for (i = 0; i < length1; i++)        /* for all characters in string */
   {
      /*****************************/
      /* special character newline */
      /*****************************/

      if (nl_valid == TRUE && *(string1 + i) == OmCHAR_NLN)
      {  *(string2 + i) = *(string1 + i);
	 continue;
      }

      /**********************************************/
      /* special characters in case of search value */
      /**********************************************/

      if (is_search == TRUE)            /* if search value */
      {
	 /* set devaluation flag */

	 {  devalold = deval;           /* save devaluation flag value */
	    if (deval == TRUE)          /* if devaluation flag is set */
	       deval = FALSE;           /* reset devaluation flag */
	    else if (deval == FALSE && *(string1 + i) == OmCHAR_DV)
					/* else if devaluation flag is reset */
					/* and deval. character found */
	       deval = TRUE;            /* set devaluation flag */
	 }

	 /* test for valid special characters */

	 if ((*(string1 + i) == OmCHAR_WC1 || *(string1 + i) == OmCHAR_WC2
	     || *(string1 + i) == OmCHAR_DV)  && devalold == FALSE)
					/* if special character and no */
					/* devaluation character preceding */
	 {  *(string2 + i) = *(string1 + i);
					/* copy input to output string */
	    continue;                   /* go to next charcter in input */
	 }
	 else if (*(string1 + i) == OmCHAR_BLK && devalold == TRUE)
					/* if devaluated blank (disallowed */
					/* in view of removing blanks) */
	 {  *(string2 + i) = OmCHAR_SUB;
					/* copy subst. character to output */
	    invnu++;                    /* increment nu. of inv. char. */
	    continue;                   /* go to next character in input */
	 }
      }

      /*************************/
      /* normal character test */
      /*************************/

      /* additional invalid codes in first position */

      if (*invfc != '\0')               /* if additional invalid codes */
					/* for first position */
      {  if (i == 0)                    /* if first position */
	 {  for (j = 0; *(invfc + j) != '\0'; j++)
					/* for all invalid codes defined */
	    {  if (*string1 == *(invfc + j))
					/* if code found in input string */
		  break;                /* leave loop */
	    }
	    if (*(invfc + j) != '\0')   /* if inv. code found in inp. string */
	    {  invnu++;                 /* incr. nu. of inv. characters */
	       *string2 = OmCHAR_SUB;   /* copy subst. character to output */
	       continue;                /* go to next character in input */
	    }
	 }
      }

      /* additional invalid codes */

      if (*invc != '\0')                /* if additional invalid codes */
      {  for (j = 0; *(invc + j) != '\0'; j++)
					/* for all invalid codes defined */
	 {  if (*(string1 + i) == *(invc + j))
					/* if code found in input string */
	       break;                   /* leave loop */
	 }
	 if (*(invc + j) != '\0')       /* if inv. code found in inp. string */
	 {  invnu++;                    /* incr. nu. of inv. characters */
	    *(string2 + i) = OmCHAR_SUB;
					/* copy subst. character to output */
	    continue;                   /* go to next character in input */
	 }
      }

      /* normal characters */

      if (( (unsigned int) omCharVal[(int) (unsigned char) *(string1 + i)]
	     & charmask) > 0)           /* if valid character */
	 *(string2 + i) = *(string1 + i);
					/* copy character to output */
      else                              /* else */
      {  invnu++;                       /* increment nu. of inv. char. */
	 *(string2 + i) = OmCHAR_SUB;   /* copy subst. char. to output */
      }
   }

   /**********/
   /* return */
   /**********/

   *length2 = length1;                  /* set length2 */
   return(invnu);                       /* return */
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharT61Length
 * DESCRIPTION:
 *    Computes the T.61 length of a string.
 * CALL SYNTAX:
 *    int OmCharT61Length(const char *string, int length)
 * PARAMETERS:
 *    IN:     string  input string (not terminated by '\0')
 *    IN:     length  length of string
 *    OUT:    return  T.61 length of string
 *
 *headerend*******************************************************************/

int OmCharT61Length(const char *string, int length)
{  int t61len;                            /* T.61 length */
   int i, j;                              /* counters */
   int c;                                 /* character value */

   if (omCharConvSwitch == FALSE)         /* if no conversion requested */
      return(length);                     /* return */

   t61len = 0;                  /* initial value for T.61 length */
   for (i = 0; i < length; i++) /* for all string characters */
   {  t61len++;                 /* basic contribution to t.61 length */
      for (j = 0; omCharExc[j][0] != 0; j++)
				/* search character in transf. exc. table */
      {  if ((unsigned char) *(string + i) == omCharExc[j][0])
	    break;
      }
      if (omCharExc[j][0] != 0) /* if character found */
      {  if (omCharExc[j][2] != 0)
				/* if two byte T.61 representation */
	    t61len++;           /* increment T.61 length */
	 continue;              /* go to next character */
      }
      c = (int) (unsigned char) *(string + i);
      if (c >= 0xa0 && c <= 0xff) /* if char. in range of conversion table 1 */
      {  if (omCharConv1[c - 0xa0][1] != 0)
				/* if double byte T.61 representation */
	    t61len++;           /* additional contribution to T.61 length */
      }
   }
   return(t61len);              /* return T.61 length */
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharAdaptPrs
 * DESCRIPTION:
 *    Adapts a string to the  Printable string character set.
 *    To do this, non-Printable-String characters are replaced by one or two
 *    substitution characters belonging to the Printable String character set.
 *    Characters which cannot be substituted are replaced by a special
 *    character.
 * CALL SYNTAX:
 *    int OmCharAdaptPrs(const char *string1, char *string2)
 * PARAMETERS:
 *    IN:     string1 input string (terminated by '\0')
 *    OUT:    string2 output string (terminated by '\0')
 *    OUT:    return  number of non-Printable-String characters
 *                    which could not be substituted
 *
 *headerend*******************************************************************/

int OmCharAdaptPrs(const char *string1, char *string2)
{  int i, j;                            /* counters */
   int c;                               /* character value */
   int c1;                              /* character value minus 0xa0 */
   int n;                               /* number of invalid characters */

   n = 0;                               /* initial value for n */
   for (i = 0, j = 0; *(string1 + i) != '\0'; i++)
					/* for all input characters */
   {  c = (int) (unsigned char) *(string1 + i);
      if (( (unsigned int) omCharVal[c] & CHS5) > 0)
					/* if Printable String character */
	 *(string2 + j++) = *(string1 + i);
					/* copy character to output */
      else if (c >= 0xa0 && c <= 0xff)  /* else if in upper code range */
      {  c1 = c - 0xa0;
	 if ((int) omCharPrs[c1][0] != 0)
					/* if substitution defined */
	 {  *(string2 + j++) = omCharPrs[c1][0];
					/* write first subst. character */
	    if ((int) omCharPrs[c1][1] != 0)
					/* if second substitution character */
	       *(string2 + j++) = omCharPrs[c1][1];
	 }                              /* write second one */
	 else                              /* else (if not defined) */
	 {  *(string2 + j++) = OmCHAR_SUB; /* copy substitution character */
	    n++;                         /* increment invalid char. counter */
	 }
      }
      else                              /* else */
      {  *(string2 + j++) = OmCHAR_SUB; /* copy substitution character */
	 n++;                           /* increment invalid char. counter */
      }
   }
   *(string2 + j) = '\0';               /* terminate string2 */
   return(n);                           /* return */
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharIsoT61
 * DESCRIPTION:
 *    Converts a string from ISO 8859 to T.61 coding.
 * CALL SYNTAX:
 *    int OmCharIsoT61(const char *string1, int length1,
 *                     char *string2, int *length2)
 * PARAMETERS:
 *    IN:     string1 input string (not terminated by '\0')
 *    IN:     length1 length of string1
 *    OUT:    string2 output string (not terminated by '\0')
 *    OUT:    length2 length of string2
 *    OUT:    return  number of invalid characters
 *
 *headerend*******************************************************************/

int OmCharIsoT61(const char *string1, int length1,
		 char *string2, int *length2)
{  register int i, j;                     /* character counters */
   int k;                                 /* counters */
   int n;                                 /* invalid values counter */
   register int c;                        /* character value */
   int ind;                               /* index in omCharConv1 */

   if (omCharConvSwitch == FALSE)         /* if no conversion requested */
   {  *length2 = length1;                 /* set output string length */
      for (i = 0; i < length1; i++)       /* copy input to output string */
	 *(string2 + i) = *(string1 + i);
      return(0);                          /* return */
   }

   for (i = 0, j = 0, n = 0; i < length1; i++)
					/* for all input characters */
   {  c = (int) (unsigned char) *(string1 + i);
      if (omCharEqual[c] == '1')        /* if code found in omCharEqual */
      {  *(string2 + j++) = *(string1 + i);
					/* copy input to output */
	 continue;                      /* go to next character */
      }
      for (k = 0; (int) omCharExc[k][0] != 0; k++)
						/* for all elements in */
						/* transf. exception table */
      {  if ((int) omCharExc[k][0] == c)        /* if code found */
	    break;                              /* leave loop */
      }
      if ((int) omCharExc[k][0] != 0)           /* if code found */
      {  *(string2 + j++) = omCharExc[k][1];    /* write first code */
	 if ((int) omCharExc[k][2] != 0)        /* if second code present */
	    *(string2 + j++) = omCharExc[k][2]; /* write second code */
	 continue;                              /* go to next character */
      }
      if (c < 0xa0)                             /* if in lower part */
      {  if (( (unsigned int) omCharVal[c] & CHS8) > 0)
						/* if valid code */
	    *(string2 + j++) = *(string1 + i);  /* copy input to output */
	 else                                   /* else */
	 {  n++;                                /* incr. inv. values counter */
	    *(string2 + j++) = OmCHAR_SUB;      /* copy substitute value */
	 }
      }
      else                                      /* else (if in higher part) */
      {  ind = c - 0xa0;
	 if (omCharConv1[ind][0] > 1)           /* if valid code */
	 {  *(string2 + j++) = omCharConv1[ind][0];
						/* copy 1st byte from table */
	    if (omCharConv1[ind][1] != 0)       /* if 2nd byte present */
	       *(string2 + j++) = omCharConv1[ind][1];
	 }                                      /* copy 2nd byte from table */
	 else                                   /* else (if invalid code) */
	 {  n++;                                /* incr. inv. values counter */
	    *(string2 + j++) = OmCHAR_SUB;      /* copy substitute value */
	 }
      }
   }
   *length2 = j;                                /* set length2 */
   return(n);                                   /* return */
}

/*function*********************************************************************
 *
 * FUNCTION NAME:
 *    OmCharT61Iso
 * DESCRIPTION:
 *    Converts a string from T.61 to ISO 8859 coding.
 * CALL SYNTAX:
 *    int OmCharT61Iso(const char *string1, int length1,
 *                     char *string2, int *length2)
 * PARAMETERS:
 *    IN:     string1 input string (not terminated by '\0')
 *    IN:     length1 length of string1
 *    OUT:    string2 output string (not terminated by '\0')
 *    OUT:    length2 length of string2
 *    OUT:    return  number of invalid characters
 *
 *headerend*******************************************************************/

int OmCharT61Iso(const char *string1, int length1,
	      char *string2, int *length2)
{  register int i, j;                     /* character counters */
   int k;                                 /* counter */
   int n;                                 /* error counter */
   register int c;                        /* code value */
   int ind;                               /* index in array omCharConv2 */

   if (omCharConvSwitch == FALSE)         /* if no conversion requested */
   {  *length2 = length1;                 /* set output string length */
      for (i = 0; i < length1; i++)       /* copy input to output string */
	 *(string2 + i) = *(string1 + i);
      return(0);                          /* return */
   }

   for (i = 0, j = 0, n = 0; i < length1; i++)
					/* for all input bytes */
   {  c = (int) (unsigned char) *(string1 + i);
      if (omCharEqual[c] == '1')        /* if code found in omCharEqual */
      {  *(string2 + j++) = *(string1 + i);
					/* copy input to output */
	 continue;                      /* go to next character */
      }
      for (k = 0; (int) omCharExc[k][0] != 0; k++)
					/* for all elements in the */
					/* transformation exception table */
      {  if ((int) omCharExc[k][1] == c)
					/* if first code found */
	 {  if ( (int) omCharExc[k][2] == 0)
					/* if second code not present */
	       break;                   /* leave loop */
	    else if ((i + 1) < length1)
					/* else if one more char. in str.1 */
	    {  if (omCharExc[k][2] == *(string1 + i + 1) )
					/* if second code is equal */
	       break;                   /* leave loop */
	    }
	 }
      }
      if ((int) omCharExc[k][0] != 0)   /* if code found */
      {  *(string2 + j++) = omCharExc[k][0];
					/* write code */
	 if ((int) omCharExc[k][2] != 0)
					/* if double code character */
	    i++;                        /* additional incr. of input counter */
	 continue;                      /* go to next character */
      }
      for (k = 0; (int) omCharExc[k][0] != 0; k++)
					/* for all elements in the */
					/* transformation exception table */
      {  if ((int) omCharExc[k][0] == c)
					/* if found as ISO 8859 code */
	    break;                      /* leave loop */
      }
      if ((int) omCharExc[k][0] != 0)   /* if character found */
      {  n++;                           /* increment error counter */
	 *(string2 + j++) = OmCHAR_SUB;
					/* write substitution character */
	 continue;                      /* go to next charcter */
      }
      if (c < 0xa0)                     /* if lower part of range */
      {  if (( (unsigned int) omCharVal[c] & CHS8) > 0)
					/* if valid code */
	    *(string2 + j++) = *(string1 + i);
					/* copy input to output */
	 else                           /* else (if invalid code) */
	 {  n++;                        /* increment error counter */
	    *(string2 + j++) = OmCHAR_SUB;
	 }                              /* copy substitute character */
      }
      else                              /* else (if upper part of range) */
      {  ind = c - 0xa0;
	 if (c < 0xc0 || c > 0xcf)      /* if 1 byte T.61 representation */
	 {  if (omCharConv2[ind] > 0)   /* if valid code */
	       *(string2 + j++) = omCharConv2[ind];
					/* write byte to output */
	    else                        /* else (if invalid code) */
	    {  n++;                     /* increase error counter */
	       *(string2 + j++) = OmCHAR_SUB;
					/* copy substitute character */
	    }
	 }
	 else                           /* else (if 2 bytes representation) */
	 {  if ((i + 1) >= length1)     /* if last character corrupted */
	    {  n++;                     /* increment error counter */
	       *(string2 + j++) = OmCHAR_SUB;
					/* copy substitution character */
	       break;                   /* finish */
	    }
	    for (k = omCharInd[c - 0xc0]; k < omCharInd[c - 0xc0 + 1]; k++)
					/* for the range in omCharConv3 */
	    {  if (omCharConv3[k][0] == *(string1 + i + 1))
					/* if 2nd T.61 byte found */
		  break;                /* leave loop */
	    }
	    if (k < omCharInd[c - 0xc0 + 1])
					/* if value found */
	       *(string2 + j++) = omCharConv3[k][1];
					/* write byte to output */
	    else                        /* else (if value not found) */
	    {  n++;                     /* increment error counter */
	       *(string2 + j++) = OmCHAR_SUB;
	    }                           /* copy substitute character */
	    i++;                        /* additional incr. of input counter */
	 }
      }
   }
   *length2 = j;
   return(n);
}

