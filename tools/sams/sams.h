/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sams.h,v $
 * Revision 1.1.6.2  1996/02/18  23:12:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:29  marty]
 *
 * Revision 1.1.6.1  1995/12/08  20:55:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:36  root]
 * 
 * Revision 1.1.4.4  1994/09/30  07:56:07  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929)
 * 	[1994/09/30  07:44:19  rsalz]
 * 
 * Revision 1.1.4.3  1994/06/10  20:53:24  devsrc
 * 	cr10872- fix copyright
 * 	[1994/06/10  16:38:10  devsrc]
 * 
 * Revision 1.1.4.2  1994/03/11  21:09:22  rsalz
 * 	Add -f flag.
 * 	Fix segfault (was freeing wrong pointer) (OT CR 10122).
 * 	[1994/03/11  14:14:19  rsalz]
 * 
 * Revision 1.1.4.1  1994/03/10  21:09:29  rsalz
 * 	Add collections and conditional text (OT CR 9697).
 * 	[1994/03/10  20:45:46  rsalz]
 * 
 * Revision 1.1.2.3  1993/11/30  16:32:58  rsalz
 * 	Put MessageID's directly into svc.c file so that it doesn't
 * 	need the msg.h file (which causes problems when using tables).
 * 	[1993/11/30  16:32:47  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  17:41:54  rsalz
 * 	Initial release
 * 	[1993/08/16  17:41:09  rsalz]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#if	!defined(TRUE)
#define TRUE	1
#define FALSE	0
#endif	/* !defined(TRUE) */


/*
**  These come from dce_error_inq_text.
*/
#define TECH_MASK		0xF0000000
#define TECH_SHIFT		28
#define ISV1_TECH		0x40000000
#define ISV2_TECH		0x50000000
#define COMP_MASK		0x0FFFF000
#define CODE_MASK		0x00000FFF
#define COMP_SHIFT		12
#define ISV_COMP_MASK		0x0FFFC000
#define ISV_CODE_MASK		0x00003FFF
#define ISV_COMP_SHIFT		14

#define REC_CHUNK		100
#define SMBUF			256
#define FNBUF			1024
#define RAD40ALPHABET		"abcdefghijklmnopqrstuvwxyz_0123456789-+@"
#define NONUM			(MSGID)0xF0F0F0F0L

#define COUNTOF(a)	(sizeof a / sizeof a[0])
#define ENDOF(a)	(&a[COUNTOF(a)])


/*
**  We could use unsigned32, but we don't want to depend on
**  having idltypes built yet.
*/
typedef unsigned long	MSGID;


/*
**  A field in a mesage definition.
*/
typedef struct msgfield_s_t {
    int			Token;
    char		*Value;
    MSGID		Number;
} msgfield_t;


/*
**  A serviceability table field.
*/
typedef struct svcfield_s_t {
    char		*Subcomp;
    char		*sc_name;
    char		*Code;
    char		*sc_descr;
    MSGID		Number;
} svcfield_t;


#define MTincatalog	0x01
#define MTintable	0x02
#define MTnodoc		0x04
#define MThidden	0x08
#define MTlongtext	0x10
#define MTsetvalue	0x20
#define MTcollect	0x40
#define MT_SKIP(f)	\
	(((f) & MThidden) != 0 || (f) == MTsetvalue || (f) == MTcollect)


/*
**  A completed message record.
*/
typedef struct record_s_t {
    MSGID		Number;
    int			Flags;
    int			TableFlag;
    int			Set;
    char		*Action;
    char		*Attributes;
    char		*Code;
    char		*Explanation;
    char		*Notes;
    char		*Subcomp;
    char		*MesgText;
    char		*DocText;
} record_t;


/*
**  Linked in later.
*/
#if	!defined(errno)
extern int	errno;
#endif	/* !defined(errno) */

extern void	yyopen(char*);
extern void	y2error(char*, char*);
extern int	yyparse(void);
extern int	yylex(void);
extern int	yyerror(char*);
extern int	Technology(const char*);


/*
**  Local globals.
*/
#if	!defined(EXTERN)
#define EXTERN	extern
#endif	/* !defined(EXTERN) */
EXTERN int		FilterText;
EXTERN int		SvcCount;
EXTERN int		SvcSize;
EXTERN svcfield_t	*Service;
EXTERN int		XPGCheck;
EXTERN int		Errors;
EXTERN int		RecCount;
EXTERN int		RecSize;
EXTERN record_t		*Records;
EXTERN char		*Table;

EXTERN char		*ServiceTable;
EXTERN char		*ServiceHandle;
EXTERN char		*CodeHeader;
EXTERN char		*CompHeader;
EXTERN char		*TechHeader;
EXTERN char		*TableHeader;
EXTERN int		ValueHeader;
EXTERN int		DefaultHeader;
extern int		CollectionSize;
