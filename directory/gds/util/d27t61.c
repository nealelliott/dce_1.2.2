/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27t61.c,v $
 * Revision 1.1.4.2  1996/02/18  18:24:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:17:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:50  root]
 * 
 * Revision 1.1.2.2  1994/07/19  14:49:04  marrek
 * 	HP-warnings removed.
 * 	[1994/07/19  12:15:39  marrek]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:44  marrek
 * 	Created in June 1994 code submission.
 * 	[1994/06/20  12:57:26  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27t61.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:24:07 $";
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
/* NAME         : d27t61.c <Functions for T61-string syntax check>    */
/*                                                                    */
/* Description  : This file contains syntax checking functions for    */
/*                T61 strings                                         */
/*                                                                    */
/* DATE         : 05/06.94                                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, BUBA NM 12                              */
/*                                                                    */
/*datoff **************************************************************/

/*--------------------------------------------------------------------*/
/*                        I N C L U D E S                             */
/*--------------------------------------------------------------------*/

#include <gds.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>

/*****  Operating System - Includes  *****/

/*****  external Includes  *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D27_LH_C_MAX 0x1F   /* Left hand control characters maximum   */
#define D27_LH_G_MAX 0x7F   /* Left hand graphic characters maximum   */
#define D27_RH_C_MAX 0x9F   /* Right hand control characters maximum  */
#define D27_RH_G_MAX 0xFF   /* Right hand graphic characters maximum  */

#define D27_HIGH_BIT (1 << 7)   /* High bit                           */

			    /* 106 control character repertoire       */
#define D27_ESCAPE   0x1B   /* Escape character                       */

#define D27_SPACE    0x20   /* Space character                        */

			    /* 102 graphic character repertoire       */
#define D27_QUOTES   0x22   /* Quotes character                       */

#define D27_PERCENT  0x25   /* Percent character                      */
#define D27_COMM_AT  0x40   /* Commercial at                          */

#define D27_LSQ_BR   0x5B   /* Left square bracket                    */
#define D27_RSQ_BR   0x5D   /* Right square bracket                   */

#define D27_UNDERLINE 0x5F  /* Underline character                    */

#define D27_VERT_LINE 0x5B  /* Vertical line                          */

#define D27_DELETE   0x7F   /* Delete character                       */

			    /* 107 control character repertoire       */
#define D27_PLD      0x8B   /* Partial line down character            */
#define D27_PLU      0x8C   /* Partial line up character              */

			    /* 103 graphic character repertoire       */
#define D27_INV_EXCLM 0xA1  /* inverted exclamation mark              */
#define D27_CURRENCY 0xA8   /* currency symbol                        */
#define D27_L_ANG_Q  0xAB   /* left angle quote                       */
#define D27_DEGREE   0xB0   /* degree character                       */
#define D27_DIVIDE   0xB8   /* degree character                       */
#define D27_R_ANG_Q  0xBB   /* right angle quote                      */
#define D27_INV_QM   0xBF   /* inverted question mark                 */

			    /* diacritical characters                 */
#define D27_GRAVE_AC 0xC1   /* grave accent                           */
#define D27_ACUTE_AC 0xC2   /* acute accent                           */
#define D27_CFLEX_AC 0xC3   /* circumflex accent                      */
#define D27_TILDE    0xC4   /* tilde                                  */
#define D27_MACRON   0xC5   /* macron                                 */
#define D27_BREVE    0xC6   /* breve                                  */
#define D27_DOT      0xC7   /* dot                                    */
#define D27_UMLAUT   0xC8   /* diaresis or umlaut                     */
#define D27_RING     0xCA   /* ring                                   */
#define D27_CEDILLA  0xCB   /* cedilla                                */
#define D27_NSP_UL   0xCC   /* non spacing underline                  */
#define D27_DACUT_AC 0xCD   /* double acute accent                    */
#define D27_OGONEK   0xCE   /* ogonek                                 */
#define D27_CARON    0xCF   /* caron                                  */

#define D27_OHM         0xE0    /* Ohm sign                           */
#define D27_CAP_AE      0xE1    /* capital AE diphtong                */
#define D27_CAP_DSTROKE 0xE2    /* capital D with stroke              */
#define D27_CAP_HSTROKE 0xE4    /* capital H with stroke              */
#define D27_CAP_IJ      0xE6    /* capital IJ ligature                */
#define D27_CAP_LDOT    0xE7    /* capital L with middle dot          */
#define D27_CAP_LSTROKE 0xE8    /* capital L with stroke              */
#define D27_CAP_OSLASH  0xE9    /* capital O with slash               */
#define D27_CAP_OE      0xEA    /* capital OE ligature                */
#define D27_CAP_THORN   0xEC    /* capital thorn, icelandic           */
#define D27_CAP_TSTROKE 0xED    /* capital T with stroke              */
#define D27_CAP_ENG     0xEE    /* capital eng, lapp                  */

#define D27_SM_DSTROKE  0xF2    /* small D with stroke                */
#define D27_SM_ETH      0xF3    /* small eth, icelandic               */
#define D27_SM_ENG      0xFE    /* small eng                          */



			    /* constants for diacritical combination  */
			    /* checking                               */
#define D27_SMALL_C  0x63
#define D27_SMALL_D  0x64
#define D27_SMALL_E  0x65
#define D27_SMALL_G  0x67
#define D27_SMALL_H  0x68
#define D27_SMALL_I  0x69
#define D27_SMALL_J  0x6a
#define D27_SMALL_K  0x6b
#define D27_SMALL_L  0x6c
#define D27_SMALL_N  0x6e
#define D27_SMALL_O  0x6f
#define D27_SMALL_R  0x72
#define D27_SMALL_S  0x73
#define D27_SMALL_T  0x74
#define D27_SMALL_U  0x75
#define D27_SMALL_W  0x77
#define D27_SMALL_Y  0x79

#define D27_CAPITAL_C 0x43
#define D27_CAPITAL_D 0x44
#define D27_CAPITAL_E 0x45
#define D27_CAPITAL_G 0x47
#define D27_CAPITAL_H 0x48
#define D27_CAPITAL_I 0x49
#define D27_CAPITAL_J 0x4a
#define D27_CAPITAL_K 0x4b
#define D27_CAPITAL_L 0x4c
#define D27_CAPITAL_N 0x4e
#define D27_CAPITAL_O 0x4f
#define D27_CAPITAL_R 0x52
#define D27_CAPITAL_S 0x53
#define D27_CAPITAL_T 0x54
#define D27_CAPITAL_U 0x55
#define D27_CAPITAL_W 0x57
#define D27_CAPITAL_Y 0x59

			    /* 126 graphic character repertoire       */
#define D27_NBSP            0xA0    /* non break space                */
#define D27_POUND           0xA3    /* pound character                */
#define D27_BROK_LINE       0xA6    /* broken line                    */
#define D27_COPY_RIGHT      0xA9    /* copy right character           */
#define D27_SHY             0xAD    /* soft hyphen                    */
#define D27_BAR             0xAF    /* bar                            */
#define D27_CAP_ALPHA_PRIME 0xB6    /* capital alpha prime            */
#define D27_CAP_EPS_PRIME   0xB8    /* capital epsilon prime          */
#define D27_CAP_ETA_PRIME   0xB9    /* capital eta prime              */
#define D27_CAP_IOTA_PRIME  0xBA    /* capital iota prime             */
#define D27_CAP_OMICR_PRIME 0xBC    /* capital omicron prime          */
#define D27_CAP_YPS_PRIME   0xBE    /* capital ypsilon prime          */
#define D27_CAP_OMEGA_PRIME 0xBF    /* capital omega prime            */
#define D27_CAPITAL_ALPHA   0xC1    /* capital alpha                  */
#define D27_CAPITAL_RHO     0xD1    /* capital rho                    */
#define D27_CAPITAL_SIGMA   0xD3    /* capital sigma                  */
#define D27_CAPITAL_YPS2DOT 0xDB    /* capital ypsilon with dots      */
#define D27_SMALL_ALPHA_PRIME 0xDC  /* small omega prime              */
#define D27_SMALL_EPS_PRIME   0xDD  /* small epsilon prime            */
#define D27_SMALL_ETA_PRIME   0xDE  /* small eta prime                */
#define D27_SMALL_IOTA_PRIME  0xDF  /* small iota prime               */
#define D27_SMALL_ALPHA       0xE1  /* small alpha                    */
#define D27_FINAL_SIGMA       0xF2  /* final sigma                    */
#define D27_SMALL_SIGMA       0xF3  /* small sigma                    */
#define D27_SMALL_OMICR_PRIME 0xFC  /* small omicron prime            */
#define D27_SMALL_YPS_PRIME   0xFD  /* small ypsilon prime            */
#define D27_SMALL_OMEGA_PRIME 0xFE  /* small omega prime              */

			    /* 87 graphic character repertoire        */
#define D27_LKANJI_1   0x30 /* lower bound for first Kanji byte       */
#define D27_LKANJI_2   0x21 /* lower bound for second Kanji byte      */

#define D27_HKANJI_1   0x74 /* upper bound for first Kanji byte       */
#define D27_HKANJI_2   0x7E /* upper bound for second Kanji byte      */

#define D27_GAP1_KANJI_1 0x4F   /* first byte for first Kanji gap     */
#define D27_GAP1_KANJI_2 0x54   /* second byte for first Kanji gap    */
#define D27_GAP2_KANJI_1 0x74   /* first byte for second Kanji gap    */
#define D27_GAP2_KANJI_2 0x25   /* second byte for second Kanji gap   */

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         DATA                */
/*                                                                    */
/**********************************************************************/

			/* charcter sequence to designate ...         */
			/* ... the supplementary graphic character    */
			/* repertoire 103 to G1                       */
static byte d27_des_103_to_G1[] = {0x29,0x75};
			/* ... the japanese graphic character         */
			/* repertoire 87 to G1                        */
static byte d27_des_87_to_G1[] = {0x24,0x29,0x42};
			/* ... the greek graphic character repertoire */
			/* 126 to G1                                  */
static byte d27_des_126_to_G1[] = {0x2C,0x46};

			/* indicator for change of char. repertoire   */
static boolean d27_ins_des_seq = FALSE;

			/* indicates position of characters relative  */
			/* to the line                                */
static signed16 d27_line_mode = 0;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

			/* checking function for ...                  */
			/* ... 102 primary graphic character ...      */
static boolean d27_631_is_102_char(byte **c);
			/* ... 103 supplementary graphic character ...*/
static boolean d27_632_is_103_char(byte **c, byte *endval);
			/* ... 106 primary control character ...      */
static boolean d27_633_is_106_char(byte **c, byte *endval);
			/* ... 107 supplementary control character ...*/
static boolean d27_634_is_107_char(byte **c);
			/* ... 126 greek graphic character ...        */
static boolean d27_635_is_87_char(byte **c, byte *endval);
			/* ... 87 japanese graphic character ...      */
static boolean d27_636_is_126_char(byte **c, byte *endval);
			/* ... repertoire                             */

			/* ... C0 control character                   */
static boolean (*d27_621_is_C0_char)(byte **c, byte *endval) =
  d27_633_is_106_char;
			/* ... C1 control character                   */
static boolean (*d27_622_is_C1_char)(byte **c) =
  d27_634_is_107_char;
			/* ... G0 graphic character                   */
static boolean (*d27_623_is_G0_char)(byte **c) = d27_631_is_102_char;
			/* ... G1 graphic character                   */
static boolean (*d27_624_is_G1_char)(byte **c, byte *endval);

			/* ... left hand control character            */
static boolean (**d27_611_is_lhc_char)(byte **c, byte *endval) =
  &d27_621_is_C0_char;
			/* ... right hand control character           */
static boolean (**d27_612_is_rhc_char)(byte **c) =
  &d27_622_is_C1_char;
			/* ... left hand graphic character            */
static boolean d27_613_is_lhg_char(byte **c);
			/* ... right hand graphic character           */
static boolean (**d27_614_is_rhg_char)(byte **c, byte *endval) =
  &d27_624_is_G1_char;

			/* norming function for ...                   */
			/* ... 103 supplementary graphic character ...*/
static void d27_682_norm_103_char(byte **in, byte **out);
			/* ... 106 primary control character ...      */
static void d27_683_norm_106_char(byte **in, byte **out);
			/* ... 107 supplementary control character ...*/
static void d27_684_norm_107_char(byte **in, byte **out);
			/* ... 87 japanese graphic character ...      */
static void d27_685_norm_87_char(byte **in, byte **out);
			/* ... 126 greek graphic character ...        */
static void d27_686_norm_126_char(byte **in, byte **out);
			/* ... repertoire                             */

			/* ... C0 control character                   */
static void (*d27_671_norm_C0_char)(byte **in, byte **out) =
  d27_683_norm_106_char;
			/* ... C1 control character                   */
static void (*d27_672_norm_C1_char)(byte **in, byte **out) =
  d27_684_norm_107_char;
			/* ... G0 graphic character                   */
static void (*d27_673_norm_G0_char)(byte **in, byte **out) =
  d27_681_norm_102_char;
			/* ... G1 graphic character                   */
static void (*d27_674_norm_G1_char)(byte **in, byte **out);

			/* ... left hand control character            */
static void (**d27_661_norm_lhc_char)(byte **in, byte **out) =
  &d27_671_norm_C0_char;
			/* ... right hand control character           */
static void (**d27_662_norm_rhc_char)(byte **in, byte **out) =
  &d27_672_norm_C1_char;
			/* ... left hand graphic character            */
static void d27_663_norm_lhg_char(byte **in, byte **out);
			/* ... right hand graphic character           */
static void (**d27_664_norm_rhg_char)(byte **in, byte **out) =
  &d27_674_norm_G1_char;

			/* compare function for T61 substrings        */
static signed16 d27_692_t61cmp(byte *str, signed16 len, byte **final_string,
    D27_codeset *codeset, byte *substr, signed16 sublen);
			/* function to reset the actual codeset       */
static void d27_693_reset_codeset(byte **str, signed16 *len,
    D27_codeset *codeset);

/**********************************************************************/
/*                                                                    */
/* Function name: d27_601_is_T61String                                */
/*                                                                    */
/* Description  : This function checks the correct syntax of a T61    */
/*                string.                                             */
/*                                                                    */
/* Return value    :                                                  */
/*   D2_NOERROR     signed32        syntax-check successful           */
/*   D2_ERROR:                      syntax-check fails                */
/*                                                                    */
/**********************************************************************/

signed32 d27_601_is_T61String(
    byte *val,          /* IN    -  string to check                   */
    signed16 len)       /* IN    -  length of string to check         */

{                       /* d27_601_is_T61String                       */

			/* end of value to check                      */
  byte *endval = val + len;

  d27_line_mode = 0;
  d27_624_is_G1_char = d27_632_is_103_char;

  while                 /* still character to check                   */
       (val < endval)
  {                     /* determine range of character               */
    if                  /* character is a left hand control character */
      ((signed16)*val <= D27_LH_C_MAX)
    {                   /* check for valid C0 character               */
      if                /* character is invalid                       */
	((*d27_611_is_lhc_char)(&val,endval) == FALSE)
      {                 /* return error                               */
	return(D2_ERROR);
      }
    }
    else if             /* character is a left hand graphic character */
	   ((signed16)*val <= D27_LH_G_MAX)
    {                   /* check for valid G0 character               */
      if                /* character is invalid                       */
	(d27_613_is_lhg_char(&val) == FALSE)
      {                 /* return error                               */
	return(D2_ERROR);
      }
    }
    else if             /* character is a right hand control character*/
	   ((signed16)*val <= D27_RH_C_MAX)
    {                   /* check for valid C1 character               */
      if                /* character is invalid                       */
	((*d27_612_is_rhc_char)(&val) == FALSE)
      {                 /* return error                               */
	return(D2_ERROR);
      }
    }
    else if             /* character is a right hand graphic character*/
	   ((signed16)*val <= D27_RH_G_MAX)
    {                   /* check for valid G1 character               */
      if                /* character is invalid                       */
	((*d27_614_is_rhg_char)(&val,endval) == FALSE)
      {                 /* return error                               */
	return(D2_ERROR);
      }
    }
  }

  return(D2_NOERROR);

}                       /* d27_601_is_T61String                       */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_613_is_lhg_char                                 */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the left hand graphic character  */
/*                set.                                                */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is valid left ...       */
/*   FALSE                          character is not a valid left ... */
/*                                  ... hand graphic character        */
/*                                                                    */
/**********************************************************************/

static boolean d27_613_is_lhg_char(
    byte **val)         /* INOUT -  character to check                */

{                       /* d27_613_is_lhg_char                        */

  if                    /* 'space' or 'delete' is encountered         */
    (**val == D27_SPACE || **val == D27_DELETE)
  {                     /* return 'character is valid'                */
    (*val)++;
    return(TRUE);
  }
  else
  {                     /* check character for valid G0               */
    return(d27_623_is_G0_char(val));
  }

}                       /* d27_613_is_lhg_char                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_631_is_102_char                                 */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the graphic character repertoire */
/*                102.                                                */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is in repertoire 102    */
/*   FALSE                          character is not in repertoire 102*/
/*                                                                    */
/**********************************************************************/

static boolean d27_631_is_102_char(
    byte **val)         /* INOUT -  character to check                */

{                       /* d27_631_is_102_char                        */

  signed16 char_val;

  char_val = (signed16)**val;
  if                    /* character is in a valid range              */
    (D27_SPACE < char_val && char_val <= D27_QUOTES ||
     D27_PERCENT <= char_val && char_val <= D27_COMM_AT ||
     D27_CAPITAL_A <= char_val && char_val <= D27_CAPITAL_Z ||
     char_val == D27_LSQ_BR || char_val == D27_RSQ_BR ||
     char_val == D27_UNDERLINE ||
     D27_SMALL_A <= char_val && char_val <= D27_SMALL_Z ||
     char_val == D27_VERT_LINE)
  {                     /* return 'is valid character'                */
    (*val)++;
    return(TRUE);
  }

  return(FALSE);

}                       /* d27_631_is_102_char                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_632_is_103_char                                 */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the graphic character repertoire */
/*                103.                                                */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is in repertoire 103    */
/*   FALSE                          character is not in repertoire 103*/
/*                                                                    */
/**********************************************************************/

static boolean d27_632_is_103_char(
    byte **val,         /* INOUT -  character to check                */
    byte *endval)       /* IN    -  end of value to check             */

{                       /* d27_632_is_103_char                        */

  signed16 char_val;

  char_val = (signed16)**val;
  if                    /* character is in a valid range              */
    (D27_INV_EXCLM <= char_val && char_val <= D27_CURRENCY ||
     char_val == D27_L_ANG_Q || D27_DEGREE <= char_val &&
     char_val <= D27_DIVIDE ||
     D27_R_ANG_Q <= char_val && char_val <= D27_INV_QM ||
     D27_OHM <= char_val && char_val <= D27_CAP_HSTROKE ||
     D27_CAP_IJ <= char_val && char_val <= D27_SM_ENG)
  {                     /* return 'is valid character'                */
    (*val)++;
    return(TRUE);
  }
  else if               /* diacritical combination could follow       */
	 (*val + 1 < endval)
  {                     /* check for diacritical characters           */
    switch(char_val)
    {
      case D27_GRAVE_AC:    /* grave accent                           */
	(*val)++;
	if                  /* valid combination with grave accent    */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_ACUTE_AC:    /* acute accent                           */
	(*val)++;
	if                  /* valid combination with acute accent    */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_C || **val == D27_CAPITAL_C ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_G ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_L || **val == D27_CAPITAL_L ||
	   **val == D27_SMALL_N || **val == D27_CAPITAL_N ||
	   **val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_R || **val == D27_CAPITAL_R ||
	   **val == D27_SMALL_S || **val == D27_CAPITAL_S ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U ||
	   **val == D27_SMALL_Y || **val == D27_CAPITAL_Y ||
	   **val == D27_SMALL_Z || **val == D27_CAPITAL_Z)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_CFLEX_AC:    /* circumflex accent                      */
	(*val)++;
	if                  /* valid combination with circumflex acc. */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_C || **val == D27_CAPITAL_C ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_G || **val == D27_CAPITAL_G ||
	   **val == D27_SMALL_H || **val == D27_CAPITAL_H ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_J || **val == D27_CAPITAL_J ||
	   **val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_S || **val == D27_CAPITAL_S ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U ||
	   **val == D27_SMALL_W || **val == D27_CAPITAL_W ||
	   **val == D27_SMALL_Y || **val == D27_CAPITAL_Y)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_TILDE:       /* tilde                                  */
	(*val)++;
	if                  /* valid combination with tilde           */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_N || **val == D27_CAPITAL_N ||
	   **val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_MACRON:      /* macron                                 */
	(*val)++;
	if                  /* valid combination with grave macron    */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_BREVE:       /* breve                                  */
	(*val)++;
	if                  /* valid combination with breve           */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_G || **val == D27_CAPITAL_G ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_DOT:         /* dot                                    */
	(*val)++;
	if                  /* valid combination with dot above       */
	  (**val == D27_SMALL_C || **val == D27_CAPITAL_C ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_G || **val == D27_CAPITAL_G ||
				  **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_Z || **val == D27_CAPITAL_Z)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_UMLAUT:      /* diaresis or umlaut                     */
	(*val)++;
	if                  /* valid combination with umlaut          */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U ||
	   **val == D27_SMALL_Y || **val == D27_CAPITAL_Y)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_RING:        /* ring                                   */
	(*val)++;
	if                  /* valid combination with ring            */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_CEDILLA:     /* cedilla                                */
	(*val)++;
	if                  /* valid combination with cedilla         */
	  (**val == D27_SMALL_C || **val == D27_CAPITAL_C ||
				  **val == D27_CAPITAL_G ||
	   **val == D27_SMALL_K || **val == D27_CAPITAL_K ||
	   **val == D27_SMALL_L || **val == D27_CAPITAL_L ||
	   **val == D27_SMALL_N || **val == D27_CAPITAL_N ||
	   **val == D27_SMALL_R || **val == D27_CAPITAL_R ||
	   **val == D27_SMALL_S || **val == D27_CAPITAL_S ||
	   **val == D27_SMALL_T || **val == D27_CAPITAL_T)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_NSP_UL:      /* non spacing underline                  */
	(*val)++;
	return(TRUE);
      case D27_DACUT_AC:    /* double acute accent                    */
	(*val)++;
	if                  /* valid combination with double acute acc*/
	  (**val == D27_SMALL_O || **val == D27_CAPITAL_O ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_OGONEK:      /* ogonek                                 */
	(*val)++;
	if                  /* valid combination with ogonek          */
	  (**val == D27_SMALL_A || **val == D27_CAPITAL_A ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_I || **val == D27_CAPITAL_I ||
	   **val == D27_SMALL_U || **val == D27_CAPITAL_U)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
      case D27_CARON:       /* caron                                  */
	(*val)++;
	if                  /* valid combination with caron           */
	  (**val == D27_SMALL_C || **val == D27_CAPITAL_C ||
	   **val == D27_SMALL_D || **val == D27_CAPITAL_D ||
	   **val == D27_SMALL_E || **val == D27_CAPITAL_E ||
	   **val == D27_SMALL_L || **val == D27_CAPITAL_L ||
	   **val == D27_SMALL_N || **val == D27_CAPITAL_N ||
	   **val == D27_SMALL_R || **val == D27_CAPITAL_R ||
	   **val == D27_SMALL_S || **val == D27_CAPITAL_S ||
	   **val == D27_SMALL_T || **val == D27_CAPITAL_T ||
	   **val == D27_SMALL_Z || **val == D27_CAPITAL_Z)
	{
	  (*val)++;
	  return(TRUE);
	}
	break;
    }
  }

  return(FALSE);

}                       /* d27_632_is_103_char                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_633_is_106_char                                 */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the control character repertoire */
/*                106. When an escape sequence                        */
/*                is encountered, the assigned right hand graphic     */
/*                character checking function is switched on.         */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is in repertoire 106    */
/*   FALSE                          character is not in repertoire 106*/
/*                                                                    */
/**********************************************************************/

static boolean d27_633_is_106_char(
    byte **val,         /* INOUT -  character to check                */
    byte *endval)       /* IN    -  end of value to check             */

{                       /* d27_633_is_106_char                        */

  if                    /* Designation character sequence encountered */
    (**val == D27_ESCAPE)
  {                     /* check following character sequence         */
    (*val)++;
    if                  /* supplementary set is designated to G1      */
      (*val + sizeof(d27_des_103_to_G1) < endval &&
       memcmp(*val,d27_des_103_to_G1,sizeof(d27_des_103_to_G1)) == 0)
    {                   /* assign function pointer                    */
      d27_624_is_G1_char = d27_632_is_103_char;
      *val += sizeof(d27_des_103_to_G1);
      return(TRUE);
    }
    else if             /* japanese character set is designated to G1 */
	   (*val + sizeof(d27_des_87_to_G1) < endval &&
	    memcmp(*val,d27_des_87_to_G1,sizeof(d27_des_87_to_G1)) == 0)
    {                   /* assign function pointer                    */
      d27_624_is_G1_char = d27_635_is_87_char;
      *val += sizeof(d27_des_87_to_G1);
      return(TRUE);
    }
    else if             /* greek character set is designated to G1    */
	   (*val + sizeof(d27_des_126_to_G1) < endval &&
	    memcmp(*val,d27_des_126_to_G1,sizeof(d27_des_126_to_G1)) == 0)
    {                   /* assign function pointer                    */
      d27_624_is_G1_char = d27_636_is_126_char;
      *val += sizeof(d27_des_126_to_G1);
      return(TRUE);
    }
  }

  return(FALSE);

}                       /* d27_633_is_106_char                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_634_is_107_char                                 */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the control character repertoire */
/*                107.                                                */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is in repertoire 107    */
/*   FALSE                          character is not in repertoire 107*/
/*                                                                    */
/**********************************************************************/

static boolean d27_634_is_107_char(
    byte **val)         /* INOUT -  character to check                */

{                       /* d27_634_is_107_char                        */

			/* see T.61, p. 256                           */
  if                    /* 'partial line down' is encountered and     */
			/* characters ar not below the line already   */
    (**val == D27_PLD && d27_line_mode >= 0)
  {                     /* reset line mode                            */
    (*val)++;
    d27_line_mode--;
    return(TRUE);
  }
  else if               /* 'partial line up' is encountered and       */
			/* characters ar not above the line already   */
    (**val == D27_PLU && d27_line_mode <= 0)
  {                     /* reset line mode                            */
    (*val)++;
    d27_line_mode++;
    return(TRUE);
  }

  return(FALSE);

}                       /* d27_634_is_107_char                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_636_is_126_char                                 */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the graphic character repertoire */
/*                126 (greek characters).                             */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is in repertoire 126    */
/*   FALSE                          character is not in repertoire 126*/
/*                                                                    */
/**********************************************************************/

static boolean d27_636_is_126_char(
    byte **val,         /* INOUT -  character to check                */
    byte *endval)       /* IN    -  end of value to check             */

{                       /* d27_636_is_126_char                        */

  signed16 char_val;

  char_val = (signed16)**val;
  if                    /* character is in a valid range              */
    (D27_NBSP <= char_val && char_val <= D27_POUND ||
     D27_BROK_LINE <= char_val && char_val <= D27_COPY_RIGHT ||
     D27_L_ANG_Q <= char_val && char_val <= D27_SHY ||
     D27_BAR <= char_val && char_val <= D27_CAPITAL_RHO ||
     D27_CAPITAL_SIGMA <= char_val && char_val <= D27_SMALL_OMEGA_PRIME)
  {                     /* return 'is valid character'                */
    (*val)++;
    return(TRUE);
  }

  return(FALSE);

}                       /* d27_636_is_126_char                        */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_635_is_87_char                                  */
/*                                                                    */
/* Description  : This function checks whether the parameter is a     */
/*                valid character of the graphic character repertoire */
/*                87 (japanese characters).                           */
/*                                                                    */
/* Return value    :                                                  */
/*   TRUE           boolean         character is in repertoire 87     */
/*   FALSE                          character is not in repertoire 87 */
/*                                                                    */
/**********************************************************************/

static boolean d27_635_is_87_char(
    byte **val,         /* INOUT -  character to check                */
    byte *endval)       /* IN    -  end of value to check             */

{                       /* d27_635_is_87_char                         */

  signed16 byte1, byte2;    /* the two bytes to check                     */

  if                    /* two bytes follwing                         */
    (*val + 1 < endval)
  {                     /* assign the two checking bytes              */
    byte1 = (signed16)(*val)[0];
    byte2 = (signed16)(*val)[1];
    if                  /* both high bits are set                     */
      ((byte1 & D27_HIGH_BIT) != 0 && (byte2 & D27_HIGH_BIT) != 0)
    {                   /* mask high bits out                         */
      byte1 &= ~D27_HIGH_BIT;
      byte2 &= ~D27_HIGH_BIT;
      if                /* bytes represent a Kanji character          */
	(D27_LKANJI_1 <= byte1 && byte1 <= D27_HKANJI_1 &&
	 D27_LKANJI_2 <= byte2 && byte2 <= D27_HKANJI_2 &&
	 !(byte1 == D27_GAP1_KANJI_1 && D27_GAP1_KANJI_2 <= byte2) &&
	 !(byte1 == D27_GAP2_KANJI_1 && D27_GAP2_KANJI_2 <= byte2))
      {                 /* return 'is 87 graphic character'           */
	(*val) += 2;
	return(TRUE);
      }
    }
  }

  return(FALSE);

}                       /* d27_635_is_87_char                         */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_651_norm_T61String                              */
/*                                                                    */
/* Description  : This function norms an attribute value with T61     */
/*                syntax.                                             */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

void d27_651_norm_T61String(
    byte *in,           /* IN    -  string to norm                    */
    byte *out,          /* OUT   -  normed string                     */
    signed16 len)       /* IN    -  length of string to norm          */

{                       /* d27_651_norm_T61String                     */

			/* end of value to check                      */
  byte *endval = in + len;

  d27_674_norm_G1_char = d27_682_norm_103_char;

  while                 /* still character to check                   */
       (in < endval)
  {                     /* determine range of character               */
    if                  /* character is a left hand control character */
      ((signed16)*in <= D27_LH_C_MAX)
    {                   /* norm it                                    */
      (*d27_661_norm_lhc_char)(&in,&out);
    }
    else if             /* character is a left hand graphic character */
	   ((signed16)*in <= D27_LH_G_MAX)
    {                   /* norm it                                    */
      d27_663_norm_lhg_char(&in,&out);
    }
    else if             /* character is a right hand control character*/
	   ((signed16)*in <= D27_RH_C_MAX)
    {                   /* norm it                                    */
      (*d27_662_norm_rhc_char)(&in,&out);
    }
    else if             /* character is a right hand graphic character*/
	   ((signed16)*in <= D27_RH_G_MAX)
    {                   /* norm it                                    */
      (*d27_664_norm_rhg_char)(&in,&out);
    }
  }

}                       /* d27_651_norm_T61String                     */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_663_norm_lhg_char                               */
/*                                                                    */
/* Description  : This function norms a left hand graphic character   */
/*                set.                                                */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

static void d27_663_norm_lhg_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_663_norm_lhg_char                      */

  if                    /* 'space' or 'delete' is encountered         */
    (**in == D27_SPACE || **in == D27_DELETE)
  {                     /* return 'character is valid'                */
    *(*out)++ = *(*in)++;
  }
  else
  {                     /* norm the G0 character                      */
    d27_673_norm_G0_char(in,out);
  }

}                       /* d27_663_norm_lhg_char                      */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_681_norm_102_char                               */
/*                                                                    */
/* Description  : This function norms a character of the graphic      */
/*                character repertoire 102.                           */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

void d27_681_norm_102_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_681_norm_102_char                      */

  if                    /* character is in upper case                 */
    (D27_CAPITAL_A <= (signed16)**in && (signed16)**in <= D27_CAPITAL_Z)
  {                     /* convert to lower case                      */
    *(*out)++ = *(*in)++ + (D27_SMALL_A - D27_CAPITAL_A);
  }
  else
  {                     /* just assign character                      */
    *(*out)++ = *(*in)++;
  }

}                       /* d27_681_norm_102_char                      */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_682_norm_103_char                               */
/*                                                                    */
/* Description  : This function norms a character of the graphic      */
/*                character repertoire 103.                           */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

static void d27_682_norm_103_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_682_norm_103_char                      */

  if                    /* character repertoire has changed           */
    (d27_ins_des_seq == TRUE)
  {                     /* insert designation sequence                */
    *(*out)++ = D27_ESCAPE;
    memcpy(*out,d27_des_103_to_G1,sizeof(d27_des_103_to_G1));
    *out += sizeof(d27_des_103_to_G1);
    d27_ins_des_seq = FALSE;
  }
  switch(**in)
  {
    case D27_CAP_AE:
    case D27_CAP_DSTROKE:
    case D27_CAP_HSTROKE:
    case D27_CAP_IJ:
    case D27_CAP_LDOT:
    case D27_CAP_LSTROKE:
    case D27_CAP_OSLASH:
    case D27_CAP_OE:
    case D27_CAP_THORN:
    case D27_CAP_TSTROKE:
    case D27_CAP_ENG:
      *(*out)++ = *(*in)++ + (D27_SM_DSTROKE - D27_CAP_DSTROKE);
      break;
    case D27_SM_ETH:
      *(*out)++ = *(*in)++ + (D27_SM_DSTROKE - D27_SM_ETH);
      break;
    default:
      *(*out)++ = *(*in)++;
      break;
  }

}                       /* d27_682_norm_103_char                      */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_683_norm_106_char                               */
/*                                                                    */
/* Description  : This function norms a character of the control      */
/*                character repertoire 106. When an escape sequence   */
/*                is encountered, the assigned right hand graphic     */
/*                character norming function is switched on.          */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

static void d27_683_norm_106_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_683_norm_106_char                      */

  if                    /* Designation character sequence encountered */
    (**in == D27_ESCAPE)
  {                     /* check following character sequence         */
    (*in)++;
    if                  /* supplementary set is designated to G1      */
      (memcmp(*in,d27_des_103_to_G1,sizeof(d27_des_103_to_G1)) == 0)
    {                   /* assign function pointer                    */
      d27_ins_des_seq = TRUE;
      d27_674_norm_G1_char = d27_682_norm_103_char;
      *in  += sizeof(d27_des_103_to_G1);
    }
    else if             /* japanese character set is designated to G1 */
	   (memcmp(*in,d27_des_87_to_G1,sizeof(d27_des_87_to_G1)) == 0)
    {                   /* assign function pointer                    */
      d27_ins_des_seq = TRUE;
      d27_674_norm_G1_char = d27_685_norm_87_char;
      *in  += sizeof(d27_des_87_to_G1);
    }
    else if             /* greek character set is designated to G1    */
	   (memcmp(*in,d27_des_126_to_G1,sizeof(d27_des_126_to_G1)) == 0)
    {                   /* assign function pointer                    */
      d27_ins_des_seq = TRUE;
      d27_674_norm_G1_char = d27_686_norm_126_char;
      *in  += sizeof(d27_des_126_to_G1);
    }
    else
    {                   /* just copy escape character                 */
      *(*out)++ = D27_ESCAPE;
    }
  }
  else
  {                     /* just copy character                        */
    *(*out)++ = *(*in)++;
  }

}                       /* d27_683_norm_106_char                      */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_684_norm_107_char                               */
/*                                                                    */
/* Description  : This function norms a character of the control      */
/*                character repertoire 107.                           */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

static void d27_684_norm_107_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_684_norm_107_char                      */

  *(*out)++ = *(*in)++;

}                       /* d27_684_norm_107_char                      */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_685_norm_87_char                                */
/*                                                                    */
/* Description  : This function norms a character of the graphic      */
/*                character repertoire 87 (japanese characters).      */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

static void d27_685_norm_87_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_685_norm_87_char                       */

  if                    /* character repertoire has changed           */
    (d27_ins_des_seq == TRUE)
  {                     /* insert designation sequence                */
    *(*out)++ = D27_ESCAPE;
    memcpy(*out,d27_des_87_to_G1,sizeof(d27_des_87_to_G1));
    *out += sizeof(d27_des_87_to_G1);
    d27_ins_des_seq = FALSE;
  }
  *(*out)++ = *(*in)++;
  *(*out)++ = *(*in)++;

}                       /* d27_685_norm_87_char                       */

/**********************************************************************/
/*                                                                    */
/* Function name: d27_686_norm_126_char                               */
/*                                                                    */
/* Description  : This function norms a character of the graphic      */
/*                character repertoire 126 (greek characters).        */
/*                                                                    */
/* Return value    :                                                  */
/*   None                                                             */
/*                                                                    */
/**********************************************************************/

static void d27_686_norm_126_char(
    byte **in,          /* INOUT -  character to norm                 */
    byte **out)         /* INOUT -  normed character                  */

{                       /* d27_686_norm_126_char                      */

  if                    /* character repertoire has changed           */
    (d27_ins_des_seq == TRUE)
  {                     /* insert designation sequence                */
    *(*out)++ = D27_ESCAPE;
    memcpy(*out,d27_des_126_to_G1,sizeof(d27_des_126_to_G1));
    *out += sizeof(d27_des_126_to_G1);
    d27_ins_des_seq = FALSE;
  }

  if                    /* character is normal upper case greek       */
    (D27_CAPITAL_ALPHA <= (signed16)**in &&
     (signed16)**in <= D27_CAPITAL_YPS2DOT)
  {                     /* add fixed offset                           */
    **out = **in + (D27_SMALL_ALPHA - D27_CAPITAL_ALPHA);
  }
  else
  {                     /* check for special cases                    */
    switch(**in)
    {
      case D27_CAP_ALPHA_PRIME:
	**out = D27_SMALL_ALPHA_PRIME;
	break;
      case D27_CAP_EPS_PRIME:
	**out = D27_SMALL_EPS_PRIME;
	break;
      case D27_CAP_ETA_PRIME:
	**out = D27_SMALL_ETA_PRIME;
	break;
      case D27_CAP_IOTA_PRIME:
	**out = D27_SMALL_IOTA_PRIME;
	break;
      case D27_CAP_OMICR_PRIME:
	**out = D27_SMALL_OMICR_PRIME;
	break;
      case D27_CAP_YPS_PRIME:
	**out = D27_SMALL_YPS_PRIME;
	break;
      case D27_CAP_OMEGA_PRIME:
	**out = D27_SMALL_OMEGA_PRIME;
	break;
      case D27_FINAL_SIGMA:
	**out = D27_SMALL_SIGMA;
	break;
      default:
	**out = **in;
	break;
    }
  }

  (*in)++;
  (*out)++;

}                       /* d27_686_norm_126_char                      */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d27_691_t61grep                                     */
/*                                                                    */
/* DATE         : 05/31/94                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether a T61 substring occurs */
/*                in a given T61 string.                              */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       signed16        match was found               */
/*      D2_NFOUND                       no match                      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d27_691_t61grep(
    byte         **str, /* INOUT -  string to be checked for substr.  */
    signed16     *len,  /* INOUT -  length of string to be checked    */
			/* INOUT -  actual codeset of string          */
    D27_codeset  *codeset,
			/* IN    -  position of proposed substring    */
    signed16     position,
			/* IN    -  proposed substring                */
    byte         *substr,
    signed16     sublen)/* IN    -  length of proposed substring      */

{                   /* d27_691_t61grep                                */

  byte     *final_string;           /* pointer to string after match  */
  D27_codeset final_codeset;        /* cdeset in string after match   */

  final_codeset = *codeset;
  switch(position)
  {
    case D2_INITIAL:
      if            /* initial substring does not match               */
	(d27_692_t61cmp(*str,*len,&final_string,&final_codeset,substr,
	 sublen))
      {
	return(D2_NFOUND);
      }
      else
      {             /* reset string pointer and length                */
	*len -= final_string - *str;
	*str  = final_string;
	*codeset = final_codeset;
	return(D2_NO_ERR);
      }
    case D2_ANY:
    case D2_FINAL:
      while         /* still characters to compare                    */
	   (*len > 0)
      {
	if          /* escape sequence is following                   */
	  (**str == D27_ESCAPE)
	{           /* reset codeset                                  */
	  (*str)++;
	  (*len)--;
	  d27_693_reset_codeset(str,len,codeset);
	  final_codeset = *codeset;
	}
	else
	{           /* initialize final string                        */
	  final_string = *str;
	  if        /* match is found                                 */
	    (d27_692_t61cmp(final_string,*len - (final_string - *str),
	     &final_string,&final_codeset,substr,sublen) == 0)
	  {         /* check position                                 */
	    if      /* match is found                                 */
	      (position == D2_ANY ||
	       position == D2_FINAL && final_string == *str + *len)
	    {       /* reset string and length and break loop         */
	      *len    -= final_string - *str;
	      *str     = final_string;
	      *codeset = final_codeset;
	      return(D2_NO_ERR);
	    }
	  }
	  else
	  {         /* reset pointer                                  */
	    (*str)++;
	    (*len)--;
	  }
	}
      }
      return(D2_NFOUND);
  }

}               /* d27_691_t61grep                                    */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d27_692_t61cmp                                      */
/*                                                                    */
/* DATE         : 05/31/94                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether a T61 substring matches*/
/*                a given T61 string.                                 */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       signed16        match was found               */
/*      D2_NFOUND                       no match                      */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d27_692_t61cmp(
    byte         *str,  /* IN    -  string to be checked for substr.  */
    signed16     len,   /* IN    -  length of string to be checked    */
			/* OUT   -  pointer to string after match     */
    byte         **final_string,
			/* INOUT -  actual codeset of string          */
    D27_codeset  *codeset,
			/* IN    -  proposed substring                */
    byte         *substr,
    signed16     sublen)/* IN    -  length of proposed substring      */

{                   /* d27_692_t61cmp                                 */

  byte *endsubstr;                  /* end of substring               */
  byte *endstr;                     /* end of string                  */
				    /* indicator for active codeset in*/
  D27_codeset ss_codeset;           /* substring                      */

  endsubstr = substr + sublen;
  endstr    = str    + len;

  for                   /* all characters in string                   */
     (*final_string = str, ss_codeset = D27_CS_103;
      *final_string < endstr && substr < endsubstr; )
  {                     /* check characters                           */
    if                  /* string to check contains escape character  */
      (**final_string == D27_ESCAPE)
    {                   /* compare sequences                          */
      (*final_string)++;
      len--;
      d27_693_reset_codeset(final_string,&len,codeset);
    }
    else if             /* substring contains escape character        */
      (*substr == D27_ESCAPE)
    {                   /* compare sequences                          */
      substr++;
      sublen--;
      d27_693_reset_codeset(&substr,&sublen,&ss_codeset);
    }
    else if             /* characters differ or characters are right  */
			/* hand characters but codesets differ        */
      (**final_string != *substr ||
       (signed16)*substr > D27_RH_C_MAX && *codeset != ss_codeset)
    {                   /* return no match                            */
      return(D2_NFOUND);
    }
    else
    {                   /* reset string pointers                      */
      (*final_string)++;
      substr++;
      len--;
      sublen--;
    }
  }

  return(substr < endsubstr ? D2_NFOUND : D2_NO_ERR);

}               /* d27_692_t61cmp                                     */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d27_693_reset_codeset                               */
/*                                                                    */
/* DATE         : 05/31/94                                            */
/*                                                                    */
/* DESCRIPTION  : The Function resets the codeset of a T61 string,    */
/*                when an escape character was encountered            */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d27_693_reset_codeset(
    byte         **str, /* INOUT -  string to be checked              */
    signed16     *len,  /* INOUT -  length of string to be checked    */
			/* OUT   -  actual codeset of string          */
    D27_codeset  *codeset)

{                   /* d27_693_reset_codeset                          */

  if                /* supplementary set is designated to G1          */
    (sizeof(d27_des_103_to_G1) < *len && memcmp(*str,d27_des_103_to_G1,
     sizeof(d27_des_103_to_G1)) == 0)
  {                 /* assign codeset                                 */
    *codeset  = D27_CS_103;
    *str     += sizeof(d27_des_103_to_G1);
    *len     -= sizeof(d27_des_103_to_G1);
  }
  else if           /* japanese character set is designated to G1     */
    (sizeof(d27_des_87_to_G1) < *len && memcmp(*str,d27_des_87_to_G1,
     sizeof(d27_des_87_to_G1)) == 0)
  {                 /* assign codeset                                 */
    *codeset  = D27_CS_87;
    *str     += sizeof(d27_des_87_to_G1);
    *len     -= sizeof(d27_des_87_to_G1);
  }
  else if           /* greek character set is designated to G1        */
    (sizeof(d27_des_126_to_G1) < *len && memcmp(*str,d27_des_126_to_G1,
     sizeof(d27_des_126_to_G1)) == 0)
  {                 /* assign codeset                                 */
    *codeset  = D27_CS_126;
    *str     += sizeof(d27_des_126_to_G1);
    len      -= sizeof(d27_des_126_to_G1);
  }

}               /* d27_693_reset_codeset                              */
