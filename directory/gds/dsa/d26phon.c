/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26phon.c,v $
 * Revision 1.1.4.2  1996/02/18  19:45:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:46:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:29  root]
 * 
 * Revision 1.1.2.2  1994/07/06  15:06:57  marrek
 * 	July 1994 code drop.
 * 	[1994/07/06  07:23:11  marrek]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:38  marrek
 * 	Created in June 1994 code submission.
 * 	[1994/06/16  11:50:09  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26phon.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:45:43 $";
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
/* NAME         : d26phon.c <Functions for phonetical matching>       */
/*                                                                    */
/* Description  : This file contains a function, that generates a     */
/*                phonetical representation of a readable string.     */
/*                                                                    */
/* DATE         : 05/30/94                                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, BUBA NM 12                              */
/*                                                                    */
/*datoff **************************************************************/

#include <d26dsa.h>
#include <dce/d27util.h>

			/* constants for phonetic string generation   */
#define D26UNDEFINED_CLASS  0x3F    /* '?'                            */
#define D26IGNORE_CLASS     0x30    /* '0'                            */

			/* object identifier for english soundex      */
			/* algorithm                                  */
#define D26_ENGLISH_SOUNDEX "\53\14\2\210\123\1\3\202\54\0"


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_soundex                                         */
/*                                                                    */
/* DESCRIPTION  : This functions generates a phonetic string for      */
/*                comparisons of equivalent strings.                  */
/*                The phonetic string is not terminated by '\0'.      */
/*                                                                    */
/* RETURNVALUE  :                                                     */
/*      D2_obj_id *         object identifier indicating the used     */
/*                          soundex algorithm                         */
/*                                                                    */
/*exoff ***************************************************************/

D2_obj_id *d26_soundex(
    byte     *str,      /* IN    -  input string                      */
    signed16 length,    /* IN    -  length of input string            */
    byte     *ph_str)   /* OUT   -  phonetical representation         */

{                               /* d26_soundex                        */

				/* object identifier for english      */
				/* soundex algorithm                  */
  static D2_obj_id soundex_alg = {sizeof(D26_ENGLISH_SOUNDEX) - 1,
				  (Ob_id_string)D26_ENGLISH_SOUNDEX};

				/* table of phonetical representations*/
				/* fo characters                      */
  static char phon_tab[] =
		   { '0',                                     /* a, A */
			  '1',                                /* b, B */
			       '2',                           /* c, C */
				    '3',                      /* d, D */
		     '0',                                     /* e, E */
			  '1',                                /* f, F */
			       '2',                           /* g, G */
		     '0',                                     /* h, H */
		     '0',                                     /* i, I */
			       '2',                           /* j, J */
			       '2',                           /* k, K */
					 '4',                 /* l, L */
					      '5',            /* m, M */
					      '5',            /* n, N */
		     '0',                                     /* o, O */
			  '1',                                /* p, P */
			       '2',                           /* q, Q */
						   '6',       /* r, R */
			       '2',                           /* s, S */
				    '3',                      /* t, T */
		     '0',                                     /* u, U */
			  '1',                                /* v, V */
		     '0',                                     /* w, W */
			       '2',                           /* x, X */
		     '0',                                     /* y, Y */
			       '2'                            /* z, Z */
		   };

  signed16   i;         /* loop variable                              */
  signed16   anz;       /* count of characters in phonetical string   */
  char       old_class; /* sound class of previous character          */
  char       class;     /* sound class of actual character            */

  if                    /* string must be converted                   */
    (str != NULL)
  {                     /* copy first character                       */
    if                  /* character is in upper case                 */
      (D27_CAPITAL_A <= (signed16)*str && (signed16)*str <= D27_CAPITAL_Z)
    {                   /* convert to lower case                      */
      *ph_str = *str + (D27_SMALL_A - D27_CAPITAL_A);
    }
    else
    {                   /* just assign character                      */
      *ph_str = *str;
    }
    if                  /* lower case character                       */
      (D27_SMALL_A <= (signed16)*ph_str && (signed16)*ph_str <= D27_SMALL_Z)
    {                   /* assign old class from phonetic table       */
      old_class = phon_tab[*str - D27_SMALL_A];
    }
    else
    {                   /* set old class undefined                    */
      old_class = D26UNDEFINED_CLASS;
    }

    str++;
    ph_str++;

    for                         /* all characters                     */
       (anz = i = 1; i < length; i++, str++)
    {
      if                        /* character is in upper case         */
	(D27_CAPITAL_A <= (signed16)*str && (signed16)*str <= D27_CAPITAL_Z)
      {                         /* determine phonetic class           */
	class = phon_tab[*str - D27_CAPITAL_A];
      }
      else if                   /* character is in lower case         */
	     (D27_SMALL_A <= (signed16)*str && (signed16)*str <= D27_SMALL_Z)
      {
	class = phon_tab[*str - D27_SMALL_A];
      }
      else
      {                           /* set class UNDEFINED              */
	class = D26UNDEFINED_CLASS;
      }

      if                        /* phonetic class is considered       */
	(class != D26IGNORE_CLASS && class != D26UNDEFINED_CLASS)
      {
	if                      /* new class != old class             */
	  (old_class != class)
	{                       /* copy character in phonetic string  */
	  *ph_str++ = class;
	  anz++;
	  if                    /* maximum length reached             */
	    (anz == D26_LPHON)
	  {                     /* break                              */
	    break;
	  }
				/* update old class by new class      */
	  old_class = class;
	}
      }
    }

    for ( ; anz < D26_LPHON; anz++)
    {
      *ph_str++ = D26IGNORE_CLASS;
    }
  }

  return(&soundex_alg);

}                               /* d26_soundex                        */
