/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: cmd_paramChecking.c,v $
 * Revision 4.9  1995/04/25  18:52:50  bab
 * [merge of changes from 4.4 to 4.5 into 4.8]
 *
 * [*** log entries omitted ***]
 *
 * $EndTALog$
 */

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: cmd_paramChecking.c,v 1.1.347.1 1996/10/02 18:47:42 damon Exp $";
#endif

/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmd_paramChecking.c,v $
 * Revision 1.1.347.1  1996/10/02  18:47:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:02  damon]
 *
 * Revision 1.1.342.5  1994/08/30  15:56:17  sasala
 * 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
 * 	[1994/08/30  00:19:58  sasala]
 * 
 * Revision 1.1.342.4  1994/08/03  16:26:57  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/02  20:25:05  ruby]
 * 
 * Revision 1.1.342.3  1994/07/13  22:29:58  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:54:09  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:17  mbs]
 * 
 * Revision 1.1.342.2  1994/06/09  14:22:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:29  annie]
 * 
 * Revision 1.1.342.1  1994/02/04  20:33:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:11  devsrc]
 * 
 * Revision 1.1.340.1  1993/12/07  17:35:53  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:18:42  jaffe]
 * 
 * Revision 1.1.2.2  1993/01/21  15:55:14  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:36  cjd]
 * 
 * Revision 1.1  1992/01/19  02:50:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*
 * cmd_paramChecking.c -- routines clients may use to do parameter validity checking
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <stdio.h>
#include <ctype.h>
#include <cmd.h>
#include <pthread.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfscmcmac.h>
#include <dfscmcsvc.h>
#include <dfscmcmsg.h>

#define HEX_PREFIX_LENGTH	(strlen("0x"))	/* both are same len */
#define STRING_IS_HEX(strP)	\
       ((strncmp((strP), "0x", HEX_PREFIX_LENGTH) == 0) || \
	(strncmp((strP), "0X", HEX_PREFIX_LENGTH) == 0))

int cmd_param_ValidateDecimalOrHexString (argStringP, flags)
     char *		argStringP;
     unsigned long	flags;
{
  int		rtnVal = 0;	/* assume success */
  char *	thisCharP = argStringP;
  int		stringIsHex = STRING_IS_HEX(argStringP);

  if (stringIsHex) {
    thisCharP += HEX_PREFIX_LENGTH;
  }
  
  while ((rtnVal == 0) && (*thisCharP != '\0')) {
    if (isdigit(*thisCharP) == 0) {
      if ((!stringIsHex) || (isxdigit(*thisCharP) == 0)) {
	rtnVal = 1;
      }
    }
    thisCharP++;
  }

  return rtnVal;
}

int cmd_param_CheckStringNumeric(argStringP, flags)
     char *			argStringP;
     unsigned long		flags;
{
  int		rtnVal = 0;
  char *	thisCharP = argStringP;
  int		allowSign;
  int		allowRadixPoint;
  int		allowCommas;

  allowSign = (flags & CMD_PARM_SIGNED) ? 1 : 0;
  allowRadixPoint = (flags & CMD_PARM_REAL) ? 1 : 0;
  allowCommas = (flags & CMD_PARM_COMMAS) ? 1 : 0;

  if ((*thisCharP == '+') || (*thisCharP == '-')) {
    if (allowSign == 0) {
      rtnVal = 1;
    }
    thisCharP++;
  }
  
  allowSign = 0;	/* we should only see one, in the first position */
  
  while ((*thisCharP != '\0') && (rtnVal == 0)) {
    if (isdigit(*thisCharP) == 0) {
      if (*thisCharP == '.') {
	if (allowRadixPoint != 0) {
	  allowRadixPoint = 0;	/* only allow one */
	}
	else {
	  rtnVal = 2;
	}	/* end else (if not allowRadixPoint) */
      }	/* end if this character is '.' */
      else {
	if ((*thisCharP != ',') || (allowCommas == 0)) {
	  rtnVal = 3;
	}	/* end if character is not a comma, or commas not allowed */
      }	/* end else (if not '.') */
    }		/* end if character is not a digit */
    thisCharP++;
  }		/* end while scanning single string */

  return rtnVal;
}

int cmd_param_CheckStringLiteral(argStringP, literalStringP)
     char *	argStringP;
     char *	literalStringP;
{
  return strcmp(argStringP, literalStringP);
}

int cmd_param_CheckDataValid(theParamDescP, dataP)
     struct cmd_parmdesc *	theParamDescP;
     char *			dataP;
{
  int			rtnVal = -1;	/* assume failure */
  cmd_parmHelpLink_t *	thisHelpP;

  if (theParamDescP->checkop) {	/* user-defined validity check */
    rtnVal = (*(theParamDescP->checkop))(dataP, theParamDescP);
  }
  else {
    for (thisHelpP = theParamDescP->firstHelp;
	 (thisHelpP != (cmd_parmHelpLink_t *)NULL) && (rtnVal != 0);
	 thisHelpP = thisHelpP->next) {
      if (thisHelpP->flags & CMD_PARM_LITERAL) {	/* literal check */
	rtnVal = cmd_param_CheckStringLiteral(dataP, thisHelpP->helpString);
      }
      else if (thisHelpP->flags & CMD_PARM_ALLNUMERICS) { /* number check */
	rtnVal = cmd_param_CheckStringNumeric(dataP, thisHelpP->flags);
      }
      else {		/* param value is unrestricted */
	rtnVal = 0;
      }
    }		/* end for traversal of help list */
  }
  
  return rtnVal;
}

int cmd_param_CheckItemsValid(theParamDescP)
     struct cmd_parmdesc *	theParamDescP;
{
  int			rtnVal = 0;
  struct cmd_item *	thisItemP;
  
  if ((theParamDescP->type == CMD_SINGLE) || (theParamDescP->type == CMD_LIST)) {
    if ((theParamDescP->wasSwitchTyped == 1) &&
	(theParamDescP->items == (struct cmd_item *)NULL)) {
      dce_printf(cmc_s_cp_null_arg,
		 theParamDescP->name,
		 ((theParamDescP->type == CMD_SINGLE) ? " a" : " at least one"));
      rtnVal = CMD_PARAM_VALIDATION_ERROR;
    }
    else {
      for (thisItemP = theParamDescP->items;
	   (thisItemP != (struct cmd_item *)NULL) && (rtnVal == 0);
	   thisItemP = thisItemP->next) {
	rtnVal = cmd_param_CheckDataValid (theParamDescP, thisItemP->data);
	if (rtnVal != 0) {
	    dce_printf(cmc_s_cp_illegal_value,
		       thisItemP->data, 
		       theParamDescP->name);
	}
      }		/* end for traversal of item list */
    }		/* end if there are any items to check */
  }		/* end if there should be any items to check */
    
  return rtnVal;
}
