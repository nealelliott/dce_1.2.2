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
 * $Log: csrc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:44:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:11:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:54  root]
 * 
 * Revision 1.1.2.3  1994/07/27  21:37:24  mori_m
 * 	CR: 11341 Use dce_strdup.
 * 	[1994/07/27  21:32:41  mori_m]
 * 
 * Revision 1.1.2.2  1994/06/10  20:53:30  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:39  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/08  21:46:28  mori_m
 * 	CR 9702:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:46:16  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <dce/dce_msg.h>

#if !defined(TRUE)
#define	TRUE	1
#define FALSE	0
#endif	/* !defined(TRUE) */

#define COUNTOF(a)	(sizeof a / sizeof a[0])
#define ENDOF(a)	(&a[COUNTOF(a)])
#define SMBUF		256
#define REC_CHUNK	100
#define CS_CSRC_MAX_CHAR_SETS		15
#define CS_CSRC_MAX_INTERMEDIATE	5
#define CS_CSRC_FOUND			0
#define CS_CSRC_NOT_FOUND		1


/* 
** A field in a message definition.
*/
typedef struct msgfield_s_t {
	int	Token;
	char	*StrValue;
	int	IntValue;
	long	LongValue;
} msgfield_t;

/*
** A default registry entry
** code_set:		OSF assigned code set id
** code_name_len:	number of bytes for the code set name
** code_set_name:	local name for the code set
** char_sets_num:	number of character sets supported by the code set
** char_sets:		array of OSF assigned character set ids
** max_bytes:		Maximum number of bytes needed to encode the code set
** priority:		preference for the intermediate code sets.
**			1 is the highest, and 5 is the lowest.  If this field
**			is 0, the code set is not used as the intermediate
**			code set.
*/
typedef struct entry_s_t {
	long		code_set;
	short		code_name_len;
	char		*code_set_name;
	short		char_sets_num;
	short		char_sets[CS_CSRC_MAX_CHAR_SETS];
	short		max_bytes;
	short		priority;
} entry_t;

/*
** A record field; occurs within each message definition.
*/
typedef struct recfield_s_t {
	int	Token;
	char	*Name;
	int	Optional;
	int	Count;
} recfield_t;

/*
** Local globals
*/
#if	!defined(EXTERN)
#define	EXTERN	extern
#endif	/* !defined(EXTERN) */
EXTERN	entry_t		*csrc_g_entries;
EXTERN	int		csrc_g_entry_count;
EXTERN	int		csrc_g_errors;
EXTERN	int		csrc_g_recsize;
EXTERN	int		csrc_g_valid_entry;
