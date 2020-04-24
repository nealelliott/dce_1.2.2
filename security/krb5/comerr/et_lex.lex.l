%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: et_lex.lex.l,v $
 * Revision 1.1.6.2  1996/03/09  23:29:34  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:44:55  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:39:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:39  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:56:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:53  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:18:35  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge UPDATE: Moved the SNI header within a % followed by
 * 	           } to fix lex/yacc problems.
 * 	[1992/09/24  15:54:08  sekhar]
 * 
 * 	[OT 5373]   SNI/SVR4 merge.
 * 	[1992/09/11  21:32:51  sekhar]
 * 
 * $EndLog$
 */
/*
*/

/*  et_lex.lex.l V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
%}
PC	[^\"]
AN	[A-Z_a-z0-9]
%%

error_table	return ERROR_TABLE;
et		return ERROR_TABLE;
error_code	return ERROR_CODE_ENTRY;
ec		return ERROR_CODE_ENTRY;
end		return END;

[\t\n ]		;

\"{PC}*\"	{ register char *p; yylval.dynstr = ds(yytext+1);
#ifdef SNI_SVR4_POSIX
		  if (p=strrchr(yylval.dynstr, '"')) *p='\0';
#else
		  if (p=rindex(yylval.dynstr, '"')) *p='\0';
#endif /* SNI_SVR4_POSIX */
		  return QUOTED_STRING;
		}

{AN}*	{ yylval.dynstr = ds(yytext); return STRING; }

#.*\n		;

.		{ return (*yytext); }
%%
#ifndef lint
static char rcsid_et_lex_lex_l[] = "$Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/et_lex.lex.l,v 1.1.6.2 1996/03/09 23:29:34 marty Exp $";
#endif
